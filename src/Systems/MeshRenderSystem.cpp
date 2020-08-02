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

    // upload per-frame values for both shaders
    m_meshRenderShaderColor->bind();
    m_registry->view<CameraComponent>().each(
            [&](auto& camera) {
            m_meshRenderShaderColor->bind();
            m_meshRenderShaderColor->upload("viewMatrix", camera.viewMatrix);
            m_meshRenderShaderColor->upload("projectionMatrix", camera.perspectiveProjection);
            });
    for (int i = 0; i < MAX_LIGHTS; i++)
        m_meshRenderShaderColor->upload("pointLights[" + std::to_string(i) + "]", m_pointLights[i]);
    m_meshRenderShaderColor->upload("dirLight", m_sun);

    m_meshRenderShaderTexture->bind();
    m_registry->view<CameraComponent>().each(
            [&](auto& camera) {
            m_meshRenderShaderTexture->bind();
            m_meshRenderShaderTexture->upload("viewMatrix", camera.viewMatrix);
            m_meshRenderShaderTexture->upload("projectionMatrix", camera.perspectiveProjection);
            });
    for (int i = 0; i < MAX_LIGHTS; i++)
        m_meshRenderShaderTexture->upload("pointLights[" + std::to_string(i) + "]", m_pointLights[i]);
    m_meshRenderShaderTexture->upload("dirLight", m_sun);

    // render
    m_registry->view<TransformationComponent, MeshRenderComponent>().each(
            [&](auto& transformation, auto& meshRenderer) {
            if (meshRenderer.geometry.getDrawCount() > 0 && meshRenderer.geometry.isReady()) {
            // render texture if diffuse map is specified
            if (meshRenderer.material->diffuseMap) {
            m_meshRenderShaderTexture->bind();
            m_meshRenderShaderTexture->upload("modelMatrix", transformation.getModelMatrix());
            m_meshRenderShaderTexture->upload("material", *meshRenderer.material);
            meshRenderer.material->diffuseMap->bind(GL_TEXTURE0);
            if (meshRenderer.material->specularMap)
            meshRenderer.material->specularMap->bind(GL_TEXTURE1);
            else
            // no specular map given, use full-white texture
            ResourceManager::getResource<Texture>("textureWhite")->bind(GL_TEXTURE1);
            } else {
            m_meshRenderShaderColor->bind();
            m_meshRenderShaderColor->upload("modelMatrix", transformation.getModelMatrix());
            m_meshRenderShaderColor->upload("material", *meshRenderer.material);
            }
            // else render color
            glBindVertexArray(meshRenderer.geometry.getVao());
            glDrawElements(GL_TRIANGLES, meshRenderer.geometry.getDrawCount(), GL_UNSIGNED_INT, nullptr);
            }
            });
}

MeshRenderSystem::MeshRenderSystem(entt::registry* registry)
    : System{registry, 0}, m_meshRenderShaderColor {ResourceManager::getResource<Shader>("shaderMeshRenderColor")},
    m_meshRenderShaderTexture {ResourceManager::getResource<Shader>("shaderMeshRenderTexture")}
{

    m_sun.direction = glm::vec3(0, -1, -1);
    m_sun.ambient = glm::vec3(0.2f);
    m_sun.diffuse = glm::vec3(0.5f);
    m_sun.specular = glm::vec3(0.2f);
}
