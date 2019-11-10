#include "pch.h"

#include "MasterRenderer.h"

void MasterRenderer::init() {
	m_blockShader.init();

	Chunk::textureAtlas.init("Resources/Textures/textureAtlas0.png");
}

void MasterRenderer::draw(const World &world, const Camera& camera) {
	// draw chunks
	m_blockShader.bind();
	Chunk::textureAtlas.bind();

	m_blockShader.uploadDirectionalLight(world.dirLight);
	m_blockShader.uploadViewMatrix(camera.getViewMatrix());
	m_blockShader.uploadProjectionMatrix(camera.getProjectionMatrix());

	for (int x = 0; x < 7; x++)
		for (int z = 0; z < 7; z++) {
			Chunk chunk = world.m_chunks[x][z];
			m_blockShader.uploadModelMatrix(glm::translate(chunk.position));

			glBindVertexArray(chunk.m_vao);
			glDrawElements(GL_TRIANGLES, chunk.m_drawCount, GL_UNSIGNED_INT, nullptr);
		}
}