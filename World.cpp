#include "pch.h"

#include "World.h"

void World::init(WorldType worldType) {
	m_generator.init(worldType);
	
	m_generator.generateTerrain(this);
}

Block World::getBlock(int x, int y, int z) const {
	return m_chunks[x / Chunk::CHUNK_SIZE][z / Chunk::CHUNK_SIZE]
		.getBlock(x % Chunk::CHUNK_SIZE, y, z % Chunk::CHUNK_SIZE);
}

void World::setBlock(int x, int y, int z, Block block) {
	m_chunks[x / Chunk::CHUNK_SIZE][z / Chunk::CHUNK_SIZE]
		.setBlock(x % Chunk::CHUNK_SIZE, y, z % Chunk::CHUNK_SIZE, block);
}

int World::getTerrainHeight(int x, int y) {
	int height = 0;
	while (getBlock(x, height, y).type != BlockType::BLOCK_AIR) height++;
	return height;
}