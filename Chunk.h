#pragma once

#include "Block.h"
#include "Vertex.h"
#include "TextureAtlas.h"

class Chunk {
private:
	// 16*16*256 array of blocks
	Block*** m_blocks;

	friend class MasterRenderer;

	GLuint m_vao;
	GLuint m_vbo, m_ebo;
	unsigned int m_drawCount;
	bool m_buffersInitialized = false;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

public:
	static TextureAtlas textureAtlas;
	glm::vec3 position;

	static const int CHUNK_SIZE = 16;
	static const int CHUNK_HEIGHT = 256;

	void init(glm::vec3 position = glm::vec3(0));
	void cleanUp();

	Block getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, Block block);

	// split up to enable multithreaded generation
	// update the geometry
	void updateVertices();

	// update the buffers
	void updateBuffers();
};
