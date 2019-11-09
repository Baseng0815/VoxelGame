#pragma once

#include "pch.h"

#include "Block.h"
#include "Vertex.h"
#include "TextureAtlas.h"

class Chunk {
private:
	// 16*16*16 array of blocks
	Block*** m_blocks;

	friend class MasterRenderer;

	GLuint m_vao;
	GLuint m_vbo, m_ebo;
	unsigned int m_drawCount;

public:
	static TextureAtlas textureAtlas;
	glm::vec3 position;

	static const int CHUNK_SIZE = 16;

	void init(glm::vec3 position = glm::vec3(0), Block fill = Block());
	void cleanUp();

	Block getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, Block block);

	// update the geometry
	void updateMesh();
};
