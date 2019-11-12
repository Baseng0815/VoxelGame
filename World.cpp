#include "pch.h"

#include "Block.h"
#include "World.h"

void World::init(WorldType worldType) {
	m_generator.init(worldType);
	
	m_generator.generate(this);
}

Block World::getBlock(int x, int y, int z) {
	return getChunk(x / Chunk::CHUNK_SIZE, z / Chunk::CHUNK_SIZE)
		->getBlock(x % Chunk::CHUNK_SIZE, y, z % Chunk::CHUNK_SIZE);
}

void World::setBlock(int x, int y, int z, Block block) {
	getChunk(x / Chunk::CHUNK_SIZE, z / Chunk::CHUNK_SIZE)
		->setBlock(x % Chunk::CHUNK_SIZE, y, z % Chunk::CHUNK_SIZE, block);
}

int World::getTerrainHeight(int x, int y) {
	int height = 0;
	while (getBlock(x, height, y).type != BlockType::BLOCK_AIR) height++;
	return height;
}

Chunk* World::createChunk(int x, int y) {
	Chunk c = Chunk();
	c.init(glm::vec3(x * Chunk::CHUNK_SIZE, 0, y * Chunk::CHUNK_SIZE), BlockType::BLOCK_AIR);

	m_chunks.push_back(c);
	return &m_chunks.back();
}

Chunk* World::getChunk(int x, int y) {
	Chunk* c = nullptr;
	int index = 0;
	while (index < m_chunks.size() && c == nullptr) {
		glm::vec3 pos = m_chunks[index].position;
		if (pos.x == x && pos.y == y) {
			c = &m_chunks[index];
		}
		index++;
	}

	return c;
}
