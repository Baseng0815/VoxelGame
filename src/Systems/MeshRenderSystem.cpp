#include "../../include/Systems/MeshRenderSystem.h"

#include "../../include/ResourceManager.h"

#include "../../include/Components/ChunkComponent.h"
#include "../../include/Components/CameraComponent.h"
#include "../../include/Components/MeshRenderComponent.h"
#include "../../include/Components/TransformationComponent.h"

#include "../../include/Resources/Shader.h"
#include "../../include/Resources/Texture.h"

void MeshRenderSystem::_update(int dt) {
    // clear screen framebuffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // upload common values for both shaders
    m_meshRenderShaderColor->bind();
    m_registry->view<CameraComponent>().each(
            [&](auto& camera) {
        m_meshRenderShaderColor->bind();
        m_meshRenderShaderColor->upload("viewMatrix", camera.viewMatrix);
        m_meshRenderShaderColor->upload("projectionMatrix", camera.perspectiveProjection);
    });

    m_meshRenderShaderTexture->bind();
    m_registry->view<CameraComponent>().each(
            [&](auto& camera) {
        m_meshRenderShaderTexture->bind();
        m_meshRenderShaderTexture->upload("viewMatrix", camera.viewMatrix);
        m_meshRenderShaderTexture->upload("projectionMatrix", camera.perspectiveProjection);
    });

    m_registry->view<TransformationComponent, MeshRenderComponent>().each(
            [&](auto& transformation, auto& meshRenderer) {
        if (meshRenderer.geometry.getDrawCount() > 0) {
            m_meshRenderShader->upload("modelMatrix", transformation.getModelMatrix());
            meshRenderer.material.diffuseMap->bind(GL_TEXTURE0);
            glBindVertexArray(meshRenderer.geometry.getVao());
            // URGENT: TODO fix this crash
            glDrawElements(GL_TRIANGLES, meshRenderer.geometry.getDrawCount(), GL_UNSIGNED_INT, nullptr);
        }
    });
}

MeshRenderSystem::MeshRenderSystem(entt::registry* registry)
    : System(registry, 0) {
        m_meshRenderShaderColor = ResourceManager::getResource<Shader>("shaderMeshRenderColor");
        m_meshRenderShaderTexture = ResourceManager::getResource<Shader>("shaderMeshRenderTexture");
    }
