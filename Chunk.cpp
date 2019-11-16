#include "pch.h"

#include "Chunk.h"
#include "Vertex.h"

TextureAtlas Chunk::textureAtlas;

void Chunk::init() {
	// allocate from heap due to its big size
	m_blocks = new Block**[CHUNK_SIZE];

	for (int x = 0; x < CHUNK_SIZE; x++) {
		m_blocks[x] = new Block*[CHUNK_HEIGHT];
		for (int y = 0; y < CHUNK_HEIGHT; y++) {
			m_blocks[x][y] = new Block[CHUNK_SIZE];
			for (int z = 0; z < CHUNK_SIZE; z++)
				m_blocks[x][y][z] = Block(BlockType::BLOCK_AIR);
		}
	}
}

void Chunk::cleanUp() {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_HEIGHT; y++)
			delete[] m_blocks[x][y];
		delete[] m_blocks[x];
	}
	delete[] m_blocks;

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

void Chunk::render(const BlockShader& shader, glm::vec2 position) const {
	shader.uploadModelMatrix(glm::translate(glm::vec3(position.x * Chunk::CHUNK_SIZE, 0, position.y * Chunk::CHUNK_SIZE)));
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, nullptr);
}

void Chunk::updateVertices() {
	int faceCount = 0;
	int faceCountPerPass = 0;

	for (int x = 0; x < CHUNK_SIZE; x++)
		for (int y = 0; y < CHUNK_HEIGHT; y++)
			for (int z = 0; z < CHUNK_SIZE; z++) {
				if (m_blocks[x][y][z].type == BlockType::BLOCK_AIR) continue;

				glm::vec3 blockPosition = glm::vec3(x, y, z);
				faceCountPerPass = 0;
				bool draw = false;

				std::array<FaceUVs, 6> faceUVs = textureAtlas.getBlockTextureCoordinates(m_blocks[x][y][z].type);

				// negative X
				if (x > 0) {
					if (m_blocks[x - 1][y][z].type == BlockType::BLOCK_AIR) draw = true;
				}
				else draw = true;

				if (draw) {
					m_vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(-1, 0, 0), faceUVs[4][0]));
					m_vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(-1, 0, 0), faceUVs[4][1]));
					m_vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(-1, 0, 0), faceUVs[4][2]));
					m_vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(-1, 0, 0), faceUVs[4][3]));

					faceCountPerPass++;
					draw = false;
				}

				// positive X
				if (x < CHUNK_SIZE - 1) {
					if (m_blocks[x + 1][y][z].type == BlockType::BLOCK_AIR) draw = true;
				}
				else draw = true;

				if (draw) {
					m_vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(1, 0, 0), faceUVs[2][0]));
					m_vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(1, 0, 0), faceUVs[2][1]));
					m_vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(1, 0, 0), faceUVs[2][2]));
					m_vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(1, 0, 0), faceUVs[2][3]));

					faceCountPerPass++;
					draw = false;
				}

				// negative Y
				if (y > 0) {
					if (m_blocks[x][y - 1][z].type == BlockType::BLOCK_AIR) draw = true;
				}
				else draw = true;

				if (draw) {
					m_vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[5][0]));
					m_vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[5][1]));
					m_vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[5][2]));
					m_vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[5][3]));

					faceCountPerPass++;
					draw = false;
				}

				// positive Y
				if (y < CHUNK_HEIGHT - 1) {
					if (m_blocks[x][y + 1][z].type == BlockType::BLOCK_AIR) draw = true;
				}
				else draw = true;

				if (draw) {
					m_vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[0][0]));
					m_vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[0][1]));
					m_vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[0][2]));
					m_vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), faceUVs[0][3]));

					faceCountPerPass++;
					draw = false;
				}

				// negative Z
				if (z > 0) {
					if (m_blocks[x][y][z - 1].type == BlockType::BLOCK_AIR) draw = true;
				}
				else draw = true;

				if (draw) {
					m_vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[3][0]));
					m_vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[3][1]));
					m_vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[3][2]));
					m_vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[3][3]));

					faceCountPerPass++;
					draw = false;
				}

				// positive Z
				if (z < CHUNK_SIZE - 1) {
					if (m_blocks[x][y][z + 1].type == BlockType::BLOCK_AIR) draw = true;
				}
				else draw = true;

				if (draw) {
					m_vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[1][0]));
					m_vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[1][1]));
					m_vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[1][2]));
					m_vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), faceUVs[1][3]));

					faceCountPerPass++;
				}

				// add m_indices
				for (int i = 0; i < faceCountPerPass; i++) {
					m_indices.push_back(faceCount * 4 + 0);
					m_indices.push_back(faceCount * 4 + 1);
					m_indices.push_back(faceCount * 4 + 2);
					m_indices.push_back(faceCount * 4 + 0);
					m_indices.push_back(faceCount * 4 + 3);
					m_indices.push_back(faceCount * 4 + 1);

					faceCount++;
				}
			}

	m_drawCount = m_indices.size();
	m_buffersOutdated = true;
}

void Chunk::updateBuffers() {
	if (m_buffersOutdated) {
		if (!m_buffersInitialized) {
			glGenVertexArrays(1, &m_vao);
			glBindVertexArray(m_vao);
			glGenBuffers(1, &m_vbo);
			glGenBuffers(1, &m_ebo);
			m_buffersInitialized = true;
		}

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_vertices.size(), m_vertices.data(), GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3) * 2));

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices[0]) * m_indices.size(), m_indices.data(), GL_DYNAMIC_DRAW);

		m_vertices.clear();
		m_indices.clear();
		
		m_buffersOutdated = false;
	}
}
