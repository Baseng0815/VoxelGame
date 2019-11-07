#include "pch.h"

#include "Chunk.h"
#include "Vertex.h"

bool Chunk::checkBounds(glm::ivec3 position) {
	return !(position.x < 0 || position.x > 15 ||
		position.y < 0 || position.y > 15 ||
		position.z < 0 || position.z > 15);
}

void Chunk::initEmpty(glm::vec3 position) {
	this->position = position;

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	// allocate from heap due to its big size
	m_blocks = new Block**[16];

	for (int x = 0; x < 16; x++) {
		m_blocks[x] = new Block * [16];

		for (int y = 0; y < 16; y++) {
			m_blocks[x][y] = new Block[16];

			for (int z = 0; z < 16; z++) {
				m_blocks[x][y][z] = Block();
			}
		}
	}

	updateMesh();
}

void Chunk::cleanUp() {
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteVertexArrays(1, &m_vao);
}

Block Chunk::getBlock(glm::ivec3 position) {
	if (checkBounds(position))
		return m_blocks[position.x][position.y][position.z];
	else
		throw std::out_of_range("Coordinates out of range.");
}

Block Chunk::getBlock(int x, int y, int z) {
	return getBlock(glm::ivec3(x, y, z));
}

void Chunk::setBlock(int x, int y, int z, Block block) {
	setBlock(glm::ivec3(x, y, z), block);
}

void Chunk::setBlock(glm::ivec3 position, Block block) {
	if (checkBounds(position))
		m_blocks[position.x][position.y][position.z] = block;
	else
		throw std::out_of_range("Coordinates out of range.");
}

void Chunk::updateMesh() {
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	int faceCount = 0;
	int faceCountPerPass = 0;

	for (int x = 0; x < 16; x++)
		for (int  y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++) {
				if (m_blocks[x][y][z].type == BLOCK_TYPE::BLOCK_AIR) continue;

				glm::vec3 blockPosition = glm::vec3(x, y, z);
				faceCountPerPass = 0;

				// negative X
				if (x > 0)
					if (m_blocks[x-1][y][z].type == BLOCK_TYPE::BLOCK_AIR) {
						vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(-1, 0, 0)));
						vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(-1, 0, 0)));
						vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(-1, 0, 0)));
						vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(-1, 0, 0)));

						faceCountPerPass++;
					}

				// positive X
				if (x < 15)
					if (m_blocks[x+1][y][z].type == BLOCK_TYPE::BLOCK_AIR) {
						vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(1, 0, 0)));
						vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(1, 0, 0)));
						vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(1, 0, 0)));
						vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(1, 0, 0)));

						faceCountPerPass++;
					}

				// negative Y
				if (y > 0)
					if (m_blocks[x][y-1][z].type == BLOCK_TYPE::BLOCK_AIR) {
						vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0)));
						vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0)));
						vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0)));
						vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0)));

						faceCountPerPass++;
					}
				
				// positive Y
				if (y < 15)
					if (m_blocks[x][y+1][z].type == BLOCK_TYPE::BLOCK_AIR) {
						vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0)));
						vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0)));
						vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0)));
						vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0)));

						faceCountPerPass++;

					}

				// negative Z
				if (z > 0)
					if (m_blocks[x][y][z-1].type == BLOCK_TYPE::BLOCK_AIR) {
						vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition));
						vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition));
						vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition));
						vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition));

						faceCountPerPass++;
					}

				// positive Z
				if (z < 15)
					if (m_blocks[x][y][z+1].type == BLOCK_TYPE::BLOCK_AIR) {
						vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition));
						vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition));
						vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition));
						vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition));

						faceCountPerPass++;
					}

				// add indices
				for (int i = 0; i < faceCountPerPass; i++) {
					indices.push_back(faceCount * 4 + 0);
					indices.push_back(faceCount * 4 + 1);
					indices.push_back(faceCount * 4 + 2);
					indices.push_back(faceCount * 4 + 0);
					indices.push_back(faceCount * 4 + 3);
					indices.push_back(faceCount * 4 + 1);

					faceCount++;
				}
			}

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3) * 2));

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);

	m_drawCount = indices.size();
}
