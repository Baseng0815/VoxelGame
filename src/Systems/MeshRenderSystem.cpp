#include "../../include/Systems/MeshRenderSystem.hpp"

#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Texture.hpp"

#include "../../include/Events/Event.hpp"
#include "../../include/Events/EventDispatcher.hpp"

#include "../../include/Components/BlockSelectionComponent.hpp"
#include "../../include/Components/CameraComponent.hpp"
#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Components/LightComponent.hpp"
#include "../../include/Components/MeshRenderComponent.hpp"
#include "../../include/Components/MultiMeshRenderComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"

void MeshRenderSystem::uploadToShader(const Shader* shader, const CameraComponent& camera, const TransformationComponent& playerTransform) const {
    shader->upload("viewMatrix", camera.viewMatrix);
    shader->upload("viewPos", camera.positionOffset + playerTransform.getPosition());
    shader->upload("projectionMatrix", camera.perspectiveProjection);

    const LightingComponent& lighting = m_registry.raw<LightingComponent>()[0];

    // upload dir lights
    shader->upload("dirLightCount", (int)lighting.dirLights.size());
    size_t lightCount = std::min(lighting.dirLights.size(), (size_t)Configuration::MAX_DIR_LIGHTS);
    for (size_t i = 0; i < lightCount; i++) {
        shader->upload("dirLights[" + std::to_string(i) + "]", lighting.dirLights[i]);
    }

    // upload point lights
    shader->upload("pointLightCount", (int)lighting.pointLights.size());
    lightCount = std::min(lighting.pointLights.size(), (size_t)Configuration::MAX_POINT_LIGHTS);
    for (size_t i = 0; i < lightCount; i++) {
        shader->upload("pointLights[" + std::to_string(i) + "]", lighting.pointLights[i]);
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
        glEnable(GL_DEPTH_TEST);
        glDrawElements(GL_TRIANGLES, meshRenderer.geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr);
    }
}

void MeshRenderSystem::_update(int dt) {
    const CameraComponent& camera = m_registry.view<CameraComponent>().raw()[0];
    const auto [playerTransform, player] = m_registry.get<TransformationComponent, PlayerComponent>(m_registry.view<PlayerComponent>().front());

    // look at block is valid
    if (player.lookAt.y > 0) {
        m_blockSelectorShader->bind();
        m_blockSelectorShader->upload("projectionMatrix", camera.perspectiveProjection);
        m_blockSelectorShader->upload("viewMatrix", camera.viewMatrix);

        const BlockSelectionComponent& blockSelector = m_registry.get<BlockSelectionComponent>(m_blockSelector);
        m_blockSelectorShader->upload("selectedBlockPos", player.lookAt);
        blockSelector.draw();
    }

    // upload per-frame values for both shaders
    // color shader
    m_meshRenderShaderColor->bind();

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
      m_meshRenderShaderTexture{ResourceManager::getResource<Shader>(SHADER_MESH_RENDER_TEXTURE)},
      m_blockSelectorShader{ResourceManager::getResource<Shader>(SHADER_BLOCK_SELECTOR)} {
    // create lighting component
    entt::entity lightEntity = m_registry.create();
    LightingComponent lighting;
    lighting.dirLights.emplace_back(DirectionalLight{glm::vec3{0.f, 0.f, 0.f}, Color{30}, Color{220}, Color{30}});
    m_registry.emplace<LightingComponent>(lightEntity, lighting);

    // create block selection entity
    m_blockSelector = m_registry.create();
    m_registry.emplace<BlockSelectionComponent>(m_blockSelector);
}
