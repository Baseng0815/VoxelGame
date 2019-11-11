#include "pch.h"

#include "MasterRenderer.h"

void MasterRenderer::init() {
	m_gBuffer.init();

	m_blockShader.init();
	m_lightingShader.init();
	m_renderQuad.init();

	Chunk::textureAtlas.init("Resources/Textures/textureAtlas0.png");
}

void MasterRenderer::resize(int width, int height) {
	glViewport(0, 0, width, height);
	m_gBuffer.resize(width, height);
}

void MasterRenderer::render(const World &world, const Camera& camera) {
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

	for (int x = 0; x < 7; x++)
		for (int z = 0; z < 7; z++) {
			Chunk chunk = world.m_chunks[x][z];
			m_blockShader.uploadModelMatrix(glm::translate(chunk.position));

			glBindVertexArray(chunk.m_vao);
			glDrawElements(GL_TRIANGLES, chunk.m_drawCount, GL_UNSIGNED_INT, nullptr);
		}

	// lighting pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_lightingShader.bind();
	m_gBuffer.bindTextures();
	
	m_lightingShader.setLight(m_light, 0);
	m_renderQuad.render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}