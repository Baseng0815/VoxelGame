#include "../include/ChunkRenderSystem.h"

#include "../include/EventDispatcher.h"

void ChunkRenderSystem::handleFramebufferSize(Event* e) {
    FramebufferSizeEvent fbsE = *e->get<FramebufferSizeEvent>();
    glViewport(0, 0, fbsE.width, fbsE.height);
    m_gBuffer.resize(fbsE.width, fbsE.height);
}

ChunkRenderSystem::ChunkRenderSystem(SystemManager* systemManager)
    : System(systemManager) {}

void ChunkRenderSystem::init() {
    ADD_EVENT(ChunkRenderSystem::handleFramebufferSize, FRAMEBUFFER_SIZE_EVENT);
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
    m_textureAtlas.bind();

    const Camera& camera = scene.player.getCamera();
    const World& world = scene.world;

    m_blockShader.uploadViewMatrix(camera.getViewMatrix());
    m_blockShader.uploadProjectionMatrix(camera.getProjectionMatrix(PROJECTION_PERSPECTIVE));

    for (auto it = world.m_chunks.begin(); it != world.m_chunks.end(); it++)
        it->second.render(m_blockShader, it->first);

    // lighting pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_lightingShader.bind();
    m_gBuffer.bindTextures();

    m_lightingShader.setLight(m_light, 0);
    m_renderQuad.render();

    m_gBuffer.bindFramebuffer(true);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, m_gBuffer.width, m_gBuffer.height, 0, 0, m_gBuffer.width, m_gBuffer.height,
            GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // skybox
    m_skyboxShader.bind();
    m_skyboxShader.uploadViewMatrix(camera.getViewMatrix());
    m_skyboxShader.uploadProjectionMatrix(camera.getProjectionMatrix(PROJECTION_PERSPECTIVE));
    m_skybox.render();

    // GUI elements
}
