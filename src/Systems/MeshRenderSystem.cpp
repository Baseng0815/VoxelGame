#include "../../include/Systems/MeshRenderSystem.hpp"

#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Texture.hpp"

#include "../../include/Events/Event.hpp"
#include "../../include/Events/EventDispatcher.hpp"

#include "../../include/Components/CameraComponent.hpp"
#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Components/MeshRenderComponent.hpp"
#include "../../include/Components/MultiMeshRenderComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"

void MeshRenderSystem::uploadToShader(const Shader* shader, const CameraComponent& camera, const TransformationComponent& playerTransform) const {
    shader->upload("viewMatrix", camera.viewMatrix);
    shader->upload("viewPos", camera.playerOffset + playerTransform.getPosition());
    shader->upload("projectionMatrix", camera.perspectiveProjection);

    for (int i = 0; i < MAX_LIGHTS; i++) {
        shader->upload("pointLights[" + std::to_string(i) + "]", m_pointLights[i]);
    }

    shader->upload("dirLight", m_sun);
    shader->setUniformState(true);
}

void MeshRenderSystem::render(const TransformationComponent& transformation, const MeshRenderComponent& meshRenderer, const CameraComponent& camera, const TransformationComponent& playerTransform) const {
    if (meshRenderer.geometry->getDrawCount() > 0) {
        const Shader* shader = meshRenderer.material->customShader;
        // no custom shader is used
        if (!shader) {
            shader = meshRenderer.material->diffuseMap ? m_meshRenderShaderTexture : m_meshRenderShaderColor;
            shader->bind();
            shader->upload("modelMatrix", transformation.getModelMatrix());
            shader->upload("material", *meshRenderer.material);

            // bind texture if diffuse map is specified
            if (meshRenderer.material->diffuseMap) {
                meshRenderer.material->diffuseMap->bind(GL_TEXTURE0);
                // use given specular map
                if (meshRenderer.material->specularMap) {
                    meshRenderer.material->specularMap->bind(GL_TEXTURE1);
                    // no specular map given, use full-white texture
                }
                else {
                    ResourceManager::getResource<Texture>(TEXTURE_WHITE)->bind(GL_TEXTURE1);
                }
            }
            // use custom shader
        }
        else {
            shader->bind();
            if (!shader->uniformsSet()) {
                uploadToShader(shader, camera, playerTransform);
                shader->setUniformState(true);
            }
            shader->upload("modelMatrix", transformation.getModelMatrix());
            shader->upload("material", *meshRenderer.material);
        }

        // some more opengl states
        if (meshRenderer.material->useBlending) {
            glEnable(GL_BLEND);
        }
        else {
            glDisable(GL_BLEND);
        }

        if (meshRenderer.material->useCulling) {
            glEnable(GL_CULL_FACE);
        }
        else {
            glDisable(GL_CULL_FACE);
        }

        // final draw call
        glBindVertexArray(meshRenderer.geometry->getVao());
        glDrawElements(GL_TRIANGLES, meshRenderer.geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr);
    }
}

void MeshRenderSystem::_update(int dt) {
    // upload per-frame values for both shaders
    // color shader
    m_meshRenderShaderColor->bind();
    const CameraComponent& camera = m_registry.view<CameraComponent>().raw()[0];
    const TransformationComponent& playerTransform = m_registry.get<TransformationComponent>(m_registry.view<PlayerComponent>().front());

    // texture shader
    m_meshRenderShaderTexture->bind();
    uploadToShader(m_meshRenderShaderTexture, camera, playerTransform);

    // color shader
    m_meshRenderShaderColor->bind();
    uploadToShader(m_meshRenderShaderColor, camera, playerTransform);

    const auto& singleMeshComponents = m_registry.view<TransformationComponent, MeshRenderComponent>();
    const auto& multiMeshComponents = m_registry.view<TransformationComponent, MultiMeshRenderComponent>();

    for (int i = 0; i < 2; i++) {
        // render single mesh components
        singleMeshComponents.each(
            [&](const TransformationComponent& transformation, const MeshRenderComponent& meshRenderer) {
                if (meshRenderer.order == i)
                    render(transformation, meshRenderer, camera, playerTransform);
            });

        // render multi mesh components
        multiMeshComponents.each(
            [&](const TransformationComponent& transformation, const MultiMeshRenderComponent& multiMeshRenderer) {
                for (const auto& meshRenderer : multiMeshRenderer.meshRenderComponents) {
                    if (meshRenderer.order == i)
                        render(transformation, meshRenderer, camera, playerTransform);
                }
            });
    }
}

MeshRenderSystem::MeshRenderSystem(Registry_T& registry)
    : System{registry, 0},
      m_meshRenderShaderColor{ResourceManager::getResource<Shader>(SHADER_MESH_RENDER_COLOR)},
      m_meshRenderShaderTexture{ResourceManager::getResource<Shader>(SHADER_MESH_RENDER_TEXTURE)} {
    m_sun.direction = glm::vec3{-0.3, -0.8, -0.5};
    m_sun.ambient = Color{70};
    m_sun.diffuse = Color{200};
    m_sun.specular = Color{30};
}
