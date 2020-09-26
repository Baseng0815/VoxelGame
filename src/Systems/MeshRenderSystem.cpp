#include "../../include/Systems/MeshRenderSystem.hpp"

#include "../../include/Resources/ResourceManager.hpp"

#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Components/CameraComponent.hpp"
#include "../../include/Components/MeshRenderComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"

#include "../../include/Resources/Texture.hpp"

void MeshRenderSystem::_update(int dt)
{
    // upload per-frame values for both shaders
    // color shader
    m_meshRenderShaderColor->bind();
    const CameraComponent &camera = m_registry.view<CameraComponent>().raw()[0];

    auto uploadToShader = [&](const Shader *shader) {
        shader->upload("viewMatrix", camera.viewMatrix);
        shader->upload("viewPos", glm::vec3 {1.f, 0.f, 0.f});
        shader->upload("projectionMatrix", camera.perspectiveProjection);

        for (int i = 0; i < MAX_LIGHTS; i++) {
            shader->upload("pointLights[" + std::to_string(i) + "]", m_pointLights[i]);
        }

        shader->upload("dirLight", m_sun);
        shader->setUniformState(true);
    };

    // texture shader
    m_meshRenderShaderTexture->bind();
    uploadToShader(m_meshRenderShaderTexture);

    // color shader
    m_meshRenderShaderColor->bind();
    uploadToShader(m_meshRenderShaderColor);

    // render
    m_registry.view<TransformationComponent, MeshRenderComponent>().each(
        [&](const TransformationComponent &transformation, const MeshRenderComponent &meshRenderer) {
            if (meshRenderer.geometry->getDrawCount() > 0) {
                const Shader *shader = meshRenderer.material->customShader;
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
                        } else {
                            ResourceManager::getResource<Texture>(TEXTURE_WHITE)->bind(GL_TEXTURE1);
                        }
                    }
                // use custom shader
                } else {
                    shader->bind();
                    if (!shader->uniformsSet()) {
                        uploadToShader(shader);
                        shader->setUniformState(true);
                    }
                    shader->upload("modelMatrix", transformation.getModelMatrix());
                    shader->upload("material", *meshRenderer.material);
                }

                // some more opengl states
                if (meshRenderer.material->useBlending) {
                    glEnable(GL_BLEND);
                } else {
                    glDisable(GL_BLEND);
                }

                if (meshRenderer.material->useCulling) {
                    glEnable(GL_CULL_FACE);
                } else {
                    glDisable(GL_CULL_FACE);
                }

                // final draw call
                glBindVertexArray(meshRenderer.geometry->getVao());
                glDrawElements(GL_TRIANGLES, meshRenderer.geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr);
            }
        });
}

MeshRenderSystem::MeshRenderSystem(Registry_T &registry)
    : System {registry, 0}, m_meshRenderShaderColor {ResourceManager::getResource<Shader>(SHADER_MESH_RENDER_COLOR)},
    m_meshRenderShaderTexture {ResourceManager::getResource<Shader>(SHADER_MESH_RENDER_TEXTURE)}
{
    m_sun.direction = glm::vec3 {-0.3, -0.8, -0.5};
    m_sun.ambient   = glm::vec3 {0.2f};
    m_sun.diffuse   = glm::vec3 {0.5f};
    m_sun.specular  = glm::vec3 {0.2f};
}
