#pragma once

#include "pch.h"

#include "Block.h"
#include "TextureAtlas.h"

class Chunk {
private:
	// 16*16*16 array of blocks
	Block*** m_blocks;

	friend class MasterRenderer;

	GLuint m_vao;
	GLuint m_vbo, m_ebo;
	unsigned int m_drawCount;

	// return false if coordinates are out of bounds
	static bool checkBounds(glm::ivec3 position);

public:
	static TextureAtlas textureAtlas;
	glm::vec3 position;

	void initEmpty(glm::vec3 position = glm::vec3(0));
	void cleanUp();

	Block getBlock(glm::ivec3 position);
	Block getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, Block block);
	void setBlock(glm::ivec3 position, Block block);

	void updateMesh();
};
