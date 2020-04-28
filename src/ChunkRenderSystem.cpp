#include "../include/ChunkRenderSystem.h"

#include "../include/SharedContext.h"
#include "../include/SystemManager.h"
#include "../include/EventDispatcher.h"

#include "../include/Components/ChunkComponent.h"
#include "../include/Components/GeometryComponent.h"
#include "../include/Components/TransformationComponent.h"

void ChunkRenderSystem::handleFramebufferSize(Event* e) {
    FramebufferSizeEvent fbsE = *e->get<FramebufferSizeEvent>();
    glViewport(0, 0, fbsE.width, fbsE.height);
    m_gBuffer.resize(fbsE.width, fbsE.height);
}

ChunkRenderSystem::ChunkRenderSystem(SystemManager* systemManager, SharedContext* context)
    : System(systemManager, context) {
    ADD_EVENT(ChunkRenderSystem::handleFramebufferSize, FRAMEBUFFER_SIZE_EVENT);

    m_light.color = glm::vec3(1, 1, 1);
    m_light.position = glm::vec3(10, 10, 10);
}

void ChunkRenderSystem::update(int dt) {
    // clear screen framebuffer
    glClearColor(0, 0, 0, 1); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // use gBuffer and clean it
    m_gBuffer.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // geometry pass: scene's geometry and color data
    m_blockShader.bind();
    m_context->textureAtlas.bind();

    m_blockShader.uploadViewMatrix(m_context->camera.getViewMatrix());
    m_blockShader.uploadProjectionMatrix(m_context->camera.getProjectionMatrix(PROJECTION_PERSPECTIVE));

    m_systemManager->getRegistry().view<TransformationComponent, GeometryComponent, ChunkComponent>().each([this]
        (auto& transformation, auto& geometry, auto& chunk) {
        if (geometry.buffersInitialized) {
            m_blockShader.uploadModelMatrix(transformation.getModelMatrix());
            glBindVertexArray(geometry.vao);
            glDrawElements(GL_TRIANGLES, geometry.drawCount, GL_UNSIGNED_INT, nullptr);
        }
    });

    // lighting pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_lightingShader.bind();
    m_gBuffer.bindTextures();

    m_lightingShader.setLight(m_light, 0);
    m_renderQuad.render();

    m_gBuffer.bindFramebuffer(true);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, m_gBuffer.getWidth(), m_gBuffer.getHeight(), 0, 0, m_gBuffer.getWidth(), m_gBuffer.getHeight(),
            GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // TODO add skybox
    /*
    m_skyboxShader.bind();
    m_skyboxShader.uploadViewMatrix(m_context->camera.getViewMatrix());
    m_skyboxShader.uploadProjectionMatrix(m_context->camera.getProjectionMatrix(PROJECTION_PERSPECTIVE));
    m_skybox.render();
    */
    // GUI elements
}
