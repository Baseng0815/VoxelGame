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

    m_meshRenderShader->bind();

    m_registry->view<CameraComponent>().each(
            [&](auto& camera) {
        m_meshRenderShader->upload("viewMatrix", camera.viewMatrix);
        m_meshRenderShader->upload("projectionMatrix", camera.perspectiveProjection);
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
        m_meshRenderShader = ResourceManager::getResource<Shader>("shaderMeshRender");
    }
