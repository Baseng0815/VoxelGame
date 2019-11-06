#include "pch.h"

#include "Chunk.h"
#include "Vertex.h"

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
				faceCountPerPass = 0;

				// negative X
				if (x > 0)
					if (m_blocks[x-1][y][z].type == BLOCK_AIR) {
						vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec3(-1, 0, 0)));
						vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(-1, 0, 0)));
						vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec3(-1, 0, 0)));
						vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-1, 0, 0)));

						faceCountPerPass++;
					}

				// positive X
				if (x < 15)
					if (m_blocks[x+1][y][z].type == BLOCK_AIR) {
						vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5), glm::vec3(1, 0, 0)));
						vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5), glm::vec3(1, 0, 0)));
						vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5), glm::vec3(1, 0, 0)));
						vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5), glm::vec3(1, 0, 0)));

						faceCountPerPass++;
					}

				// negative Y
				if (y > 0)
					if (m_blocks[x][y-1][z].type == BLOCK_AIR) {
						vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0, -1, 0)));
						vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5), glm::vec3(0, -1, 0)));
						vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5), glm::vec3(0, -1, 0)));
						vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, -1, 0)));

						faceCountPerPass++;
					}
				
				// positive Y
				if (y < 15)
					if (m_blocks[x][y+1][z].type == BLOCK_AIR) {

					}

				// negative Z
				if (z > 0)
					if (m_blocks[x][y][z-1].type == BLOCK_AIR) {

					}

				// positive Z
				if (z < 15)
					if (m_blocks[x][y][z+1].type == BLOCK_AIR) {

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

Chunk::Chunk() {
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++) {
				m_blocks[x][y][z].type = BLOCK_AIR;
			}

	updateMesh();
}

Block& Chunk::blockAt(int x, int y, int z) {
	if (x < 0 || x > 15 || y < 0 || y > 15 || z < 0 || z > 15)
		throw std::invalid_argument("Coordinates out of range.");
	else
		return m_blocks[x][y][z];
}
