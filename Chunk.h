#pragma once

#include "Block.h"
#include "Vertex.h"
#include "BlockShader.h"
#include "TextureAtlas.h"

class Chunk {
private:
	// 16*16*256 array of blocks
	Block*** m_blocks;

	GLuint m_vao;
	GLuint m_vbo, m_ebo;
	unsigned int m_drawCount;
	bool m_buffersInitialized = false;
	bool m_buffersOutdated = true;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
public:
	static TextureAtlas textureAtlas;

	static constexpr int CHUNK_SIZE = 16;
	static constexpr int CHUNK_HEIGHT = 256;

	void init();
	void cleanUp();

	Block getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, Block block);

	void render(const BlockShader& shader, glm::vec2 position) const;

	// split up to enable multithreaded generation
	// update the geometry
	void updateVertices();

	// update the buffers if needed
	void updateBuffers();
};
