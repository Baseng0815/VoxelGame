#include "pch.h"

#include "MasterRenderer.h"

void MasterRenderer::init() {
	std::vector<std::string> attribs = {
		"position",
		"normal",
		"uvCoords"
	};

	m_chunkShader.init("Resources/Shaders/chunkShader", attribs);
	m_chunkShader.addUniform("viewMatrix");
	m_chunkShader.addUniform("projectionMatrix");
	m_chunkShader.addUniform("modelMatrix");

	Chunk::textureAtlas.init("Resources/Textures/textureAtlas.png");

	std::vector<FaceUVs> a;
	for (size_t i = 0; i < 5; i++)
		a.push_back(Chunk::textureAtlas.getTextureCoordinates(i));

}

void MasterRenderer::draw(const std::vector<Chunk>& chunks, const Camera& camera) {
	// draw chunks
	m_chunkShader.bind();
	Chunk::textureAtlas.bind();

	m_chunkShader.upload("viewMatrix", camera.getViewMatrix());
	m_chunkShader.upload("projectionMatrix", camera.getProjectionMatrix());

	for (const auto& chunk : chunks) {
		m_chunkShader.upload("modelMatrix", glm::mat4(1.0f));

		glBindVertexArray(chunk.m_vao);
		glDrawElements(GL_TRIANGLES, chunk.m_drawCount, GL_UNSIGNED_INT, nullptr);
	}
}