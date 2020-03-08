#include "../include/MasterRenderer.h"
#include "../include/EventDispatcher.h"


void MasterRenderer::init() {
    m_gBuffer.init();

    m_blockShader.init();
    m_lightingShader.init();
    m_renderQuad.init();

    m_skyboxShader.init();
    m_skybox.init();

    m_textureAtlas.init("Resources/Textures/textureAtlas0.png");
    Chunk::setBlockUVsArray(m_textureAtlas.getBlockTextureCoordinates());

    ADD_EVENT(MasterRenderer::handleFramebufferSize, FRAMEBUFFER_SIZE_EVENT);

    m_guiRenderer.init();
}

void MasterRenderer::render(const Scene& scene) {
}
