#include "pch.h"

#include "Block.h"
#include "World.h"

void World::init(WorldType worldType) {
	m_generator.init(worldType);
	
	m_generator.generate(*this);

	for (int x = 0; x < Definitions::CHUNK_PRELOAD_SIZE; x++)
		for (int z = 0; z < Definitions::CHUNK_PRELOAD_SIZE; z++) {
			m_chunks[x][z].updateVertices();
			m_chunks[x][z].updateBuffers();
		}
}

World::~World() {
	for (int x = 0; x < Definitions::CHUNK_PRELOAD_SIZE; x++)
		for (int z = 0; z < Definitions::CHUNK_PRELOAD_SIZE; z++)
			m_chunks[x][z].cleanUp();
}

Block World::getBlock(int x, int y, int z) {
	return m_chunks[x / Chunk::CHUNK_SIZE][z / Chunk::CHUNK_SIZE].
		getBlock(x % Chunk::CHUNK_SIZE, y, z % Chunk::CHUNK_SIZE);
}

void World::setBlock(int x, int y, int z, Block block) {
	m_chunks[x / Chunk::CHUNK_SIZE][z / Chunk::CHUNK_SIZE].
		setBlock(x % Chunk::CHUNK_SIZE, y, z % Chunk::CHUNK_SIZE, block);
}

int World::getTerrainHeight(int x, int y) {
	int height = 0;
	while (getBlock(x, height, y).type != BlockType::BLOCK_AIR) height++;
	return height;
}
