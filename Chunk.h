#pragma once

#include "pch.h"

#include "Block.h"

class Chunk {
private:
	Block m_blocks[16][16][16];

	GLuint m_vao;
	GLuint m_vbo, m_ebo;
	unsigned int m_drawCount;

	void updateMesh();

public:
	glm::vec3 position;

	Chunk();

	void draw();

	Block& blockAt(int x, int y, int z);
};
