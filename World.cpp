#include "pch.h"

#include "Block.h"
#include "World.h"

void World::init(WorldType worldType) {
	m_generator.init(worldType);
	
	m_generator.generate(*this);

	for (auto it = m_chunks.begin(); it != m_chunks.end(); it++) {
		it->second.updateVertices();
		it->second.updateBuffers();
	}
}

World::~World() {
	for (auto it = m_chunks.begin(); it != m_chunks.end(); it++)
		it->second.cleanUp();
}

Block World::getBlock(int x, int y, int z) const {
	return m_chunks.at(glm::vec2(x / Chunk::CHUNK_SIZE, z / Chunk::CHUNK_SIZE)).
		getBlock(x % Chunk::CHUNK_SIZE, y, z % Chunk::CHUNK_SIZE);
}

void World::setBlock(int x, int y, int z, Block block) {
	m_chunks[glm::vec2(x / Chunk::CHUNK_SIZE, z / Chunk::CHUNK_SIZE)].
		setBlock(x % Chunk::CHUNK_SIZE, y, z % Chunk::CHUNK_SIZE, block);
}

int World::getTerrainHeight(int x, int y) {
	int height = 0;
	while (getBlock(x, height, y).type != BlockType::BLOCK_AIR) height++;
	return height;
}
