#include "../../include/Systems/EntityRenderSystem.h"

#include "../../include/Events/EventDispatcher.h"
#include "../../include/ResourceManager.h"

#include "../../include/Components/ChunkComponent.h"
#include "../../include/Components/CameraComponent.h"
#include "../../include/Components/TextureComponent.h"
#include "../../include/Components/GeometryComponent.h"
#include "../../include/Components/TransformationComponent.h"

#include "../../include/Rendering/Texture.h"

void EntityRenderSystem::handleFramebufferSize(const FramebufferSizeEvent& e) {
    m_gBuffer.resize(e.width, e.height);
    m_renderQuad.resize(Rectangle(0, 0, e.width, e.height));
    m_orthoProjection = glm::ortho(0.f, e.width, 0.f, e.height);
}

// TODO remove
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

    m_registry->view<CameraComponent, TransformationComponent>().each(
        [=](auto& camera, auto& transformation) {
        m_blockShader->upload("viewMatrix", camera.viewMatrix);
        m_blockShader->upload("projectionMatrix", camera.perspectiveProjection);
    });

    m_registry->view<TransformationComponent, GeometryComponent, TextureComponent>().each(
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

    /* TODO store orthoProjection somewhere else
    m_registry->view<CameraComponent>().each(
        [=](auto& camera) {
        m_lightingShader->upload("projectionMatrix", camera.orthoProjection);
    });
    */

    m_lightingShader->upload("projectionMatrix", m_orthoProjection);
    m_lightingShader->upload("lights[0].dir", m_light.dir);
    m_lightingShader->upload("lights[0].color", m_light.color);

    m_gBuffer.bindTextures();
    m_renderQuad.render();

    m_gBuffer.bindFramebuffer(true);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, m_gBuffer.getWidth(), m_gBuffer.getHeight(), 0, 0, m_gBuffer.getWidth(), m_gBuffer.getHeight(),
            GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // TODO add skybox
    /*
    m_skyboxShader->bind();
    m_skyboxShader->uploadViewMatrix(m_context->camera.getViewMatrix());
    m_skyboxShader->uploadProjectionMatrix(m_context->camera.getProjectionMatrix(PROJECTION_PERSPECTIVE));
    m_skybox.render();
    */
    // GUI elements
}

EntityRenderSystem::EntityRenderSystem(entt::registry* registry)
    : System(registry, 0), m_renderQuad(Rectangle(0, 0, Configuration::INITIAL_WINDOW_WIDTH, Configuration::INITIAL_WINDOW_HEIGHT)) {
        // add events
        EventDispatcher::onFramebufferSize += [this](const FramebufferSizeEvent& e) {
            handleFramebufferSize(e);
        };

	// TODO change naming scheme
	m_blockShader = ResourceManager::getResource<Shader>("chunkShader");
	m_lightingShader = ResourceManager::getResource<Shader>("lightingShader");
	m_atlas = ResourceManager::getResource<Texture>("textureAtlas");

	m_light.color = glm::vec3(0.7, 0.7, 0.7);
	m_light.dir = glm::vec3(0, 1, 0);
}
