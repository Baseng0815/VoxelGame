#include "../include/EntityRenderSystem.h"

#include "../include/SystemManager.h"
#include "../include/EventDispatcher.h"
#include "../include/ResourceManager.h"

#include "../include/Components/ChunkComponent.h"
#include "../include/Components/CameraComponent.h"
#include "../include/Components/TextureComponent.h"
#include "../include/Components/GeometryComponent.h"
#include "../include/Components/TransformationComponent.h"

#include "../include/Texture.h"

void EntityRenderSystem::handleFramebufferSize(Event* e) {
    FramebufferSizeEvent fbsE = *e->get<FramebufferSizeEvent>();
    glViewport(0, 0, fbsE.width, fbsE.height);
    m_gBuffer.resize(fbsE.width, fbsE.height);
}

#include <iostream>

void EntityRenderSystem::_update(int dt) {
    // clear screen framebuffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use gBuffer and clean it
    m_gBuffer.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // geometry pass: scene's geometry and color data
    m_blockShader->bind();

    m_systemManager->getRegistry().view<CameraComponent>().each(
        [=](auto& camera) {
        m_blockShader->upload("viewMatrix", camera.viewMatrix);
        m_blockShader->upload("projectionMatrix", camera.projectionMatrix);
    });

    m_systemManager->getRegistry().view<TransformationComponent, GeometryComponent, TextureComponent>().each(
        [=](auto& transformation, auto& geometry, auto& texture) {
        if (geometry.buffersInitialized) {
            m_blockShader->upload("modelMatrix", transformation.getModelMatrix());
            texture.texture->bind(GL_TEXTURE0);
            glBindVertexArray(geometry.vao);
            glDrawElements(GL_TRIANGLES, geometry.drawCount, GL_UNSIGNED_INT, nullptr);
        }
    });

    // lighting pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_lightingShader->bind();
    m_gBuffer.bindTextures();

    m_lightingShader->upload("lights[0].position", m_light.position);
    m_lightingShader->upload("lights[0].color", m_light.color);
    m_renderQuad.render();

    m_gBuffer.bindFramebuffer(true);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, m_gBuffer.getWidth(), m_gBuffer.getHeight(), 0, 0, m_gBuffer.getWidth(), m_gBuffer.getHeight(),
            GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // TODO add skybox
    /*
    m_skyboxShader->bind();
    m_skyboxShader->uploadViewMatrix(m_context->camera.getViewMatrix());
    m_skyboxShader->uploadProjectionMatrix(m_context->camera.getProjectionMatrix(PROJECTION_PERSPECTIVE));
    m_skybox.render();
    */
    // GUI elements
}

EntityRenderSystem::EntityRenderSystem(SystemManager* systemManager)
    : System(systemManager, 0) {
    ADD_EVENT(EntityRenderSystem::handleFramebufferSize, FRAMEBUFFER_SIZE_EVENT);

    // TODO change naming scheme
    m_blockShader = ResourceManager::getResource<Shader>("chunkShader");
    m_lightingShader = ResourceManager::getResource<Shader>("lightingShader");
    m_atlas = ResourceManager::getResource<Texture>("textureAtlas");

    m_light.color = glm::vec3(1, 1, 1);
    m_light.position = glm::vec3(10, 10, 10);
}
