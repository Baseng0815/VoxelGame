#include "pch.h"

#include "Chunk.h"
#include "Vertex.h"

TextureAtlas Chunk::textureAtlas;

void Chunk::init(glm::vec3 position, Block fill) {
	this->position = position;

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	// allocate from heap due to its big size
	m_blocks = new Block**[CHUNK_SIZE];

	for (int x = 0; x < CHUNK_SIZE; x++) {
		m_blocks[x] = new Block * [CHUNK_SIZE];

		for (int y = 0; y < CHUNK_SIZE; y++) {
			m_blocks[x][y] = new Block[CHUNK_SIZE];

			for (int z = 0; z < CHUNK_SIZE; z++)
				m_blocks[x][y][z] = fill;
		}
	}

	updateMesh();
}

void Chunk::cleanUp() {
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteVertexArrays(1, &m_vao);
}

Block Chunk::getBlock(int x, int y, int z) const {
	return m_blocks[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, Block block) {
	m_blocks[x][y][z] = block;
}

void Chunk::updateMesh() {
	int faceCount = 0;
	int faceCountPerPass = 0;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (int x = 0; x < CHUNK_SIZE; x++)
		for (int  y = 0; y < CHUNK_SIZE; y++)
			for (int z = 0; z < CHUNK_SIZE; z++) {
				if (m_blocks[x][y][z].type == BLOCK_AIR) continue;

				glm::vec3 blockPosition = glm::vec3(x, y, z);
				faceCountPerPass = 0;
				bool draw = false;

				std::array<FaceUVs, 6> faceUVs = textureAtlas.getBlockTextureCoordinates(m_blocks[x][y][z].type);

				// negative X
				if (x > 0) {
					if (m_blocks[x - 1][y][z].type == BLOCK_AIR) draw = true;
				} else draw = true;

				if (draw) {
					vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(-1, 0, 0), faceUVs[4][0]));
					vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(-1, 0, 0), faceUVs[4][1]));
					vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(-1, 0, 0), faceUVs[4][2]));
					vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(-1, 0, 0), faceUVs[4][3]));

					faceCountPerPass++;
					draw = false;
				}

				// positive X
				if (x < CHUNK_SIZE-1) {
					if (m_blocks[x + 1][y][z].type == BLOCK_AIR) draw = true;
				} else draw = true;

				if (draw) {
					vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(1, 0, 0), faceUVs[2][0]));
					vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(1, 0, 0), faceUVs[2][1]));
					vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(1, 0, 0), faceUVs[2][2]));
					vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(1, 0, 0), faceUVs[2][3]));

					faceCountPerPass++;
					draw = false;
				}

				// negative Y
				if (y > 0) {
					if (m_blocks[x][y - 1][z].type == BLOCK_AIR) draw = true;
				} else draw = true;

				if (draw) {
					vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[5][0]));
					vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[5][1]));
					vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[5][2]));
					vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[5][3]));

					faceCountPerPass++;
					draw = false;
				}
				
				// positive Y
				if (y < CHUNK_SIZE-1) {
					if (m_blocks[x][y + 1][z].type == BLOCK_AIR) draw = true;
				} else draw = true;

				if (draw) {
					vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[0][0]));
					vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[0][1]));
					vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[0][2]));
					vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[0][3]));

					faceCountPerPass++;
					draw = false;
				}

				// negative Z
				if (z > 0) {
					if (m_blocks[x][y][z - 1].type == BLOCK_AIR) draw = true;
				} else draw = true;

				if (draw) {
					vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[3][0]));
					vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[3][1]));
					vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[3][2]));
					vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[3][3]));

					faceCountPerPass++;
					draw = false;
				}

				// positive Z
				if (z < CHUNK_SIZE-1) {
					if (m_blocks[x][y][z + 1].type == BLOCK_AIR) draw = true;
				} else draw = true;

				if (draw) {
					vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[1][0]));
					vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[1][1]));
					vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[1][2]));
					vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[1][3]));

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

	m_drawCount = indices.size();

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3) * 2));

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
}
