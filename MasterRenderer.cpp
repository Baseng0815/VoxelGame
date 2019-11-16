#include "pch.h"

#include "MasterRenderer.h"

void MasterRenderer::init() {
	m_gBuffer.init();

	m_blockShader.init();
	m_lightingShader.init();
	m_renderQuad.init();

	m_skyboxShader.init();
	m_skybox.init();

	Chunk::textureAtlas.init("Resources/Textures/textureAtlas0.png");
}

void MasterRenderer::resize(int width, int height) {
	glViewport(0, 0, width, height);
	m_gBuffer.resize(width, height);
}

void MasterRenderer::render(const World& world, const Camera& camera) {
	// clear screen framebuffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use gBuffer and clean it
	m_gBuffer.bindFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// geometry pass: scene's geometry and color data
	m_blockShader.bind();
	Chunk::textureAtlas.bind();

	m_blockShader.uploadViewMatrix(camera.getViewMatrix());
	m_blockShader.uploadProjectionMatrix(camera.getProjectionMatrix());

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

	m_skyboxShader.bind();
	m_skyboxShader.uploadViewMatrix(camera.getViewMatrix());
	m_skyboxShader.uploadProjectionMatrix(camera.getProjectionMatrix());
	m_skybox.render();
}