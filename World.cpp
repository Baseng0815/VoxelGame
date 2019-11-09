#include "pch.h"

#include "World.h"


void World::generateChunk(int x, int z, WorldType type) {
	Chunk chunk;
	chunk.init(glm::vec3(x * Chunk::CHUNK_SIZE, 0, z * Chunk::CHUNK_SIZE));

	for (int cx = 0; cx < Chunk::CHUNK_SIZE; cx++) {
		for (int cy = 0; cy < Chunk::CHUNK_SIZE; cy++) {
			// flat world
			if (type == WORLD_FLAT) {
				Block block;
				if (cy < 12) block = Block(BLOCK_COBBLESTONE);
				else if (cy < 15) block = Block(BLOCK_DIRT);
				else block = Block(BLOCK_GRASS);

				for (int cz = 0; cz < 16; cz++)
					chunk.setBlock(cx, cy, cz, block);
			}
		}
	}

	chunk.updateMesh();
	m_chunks[x][z] = chunk;
}

void World::init(WorldType worldType) {
	for (int x = 0; x < 7; x++)
		for (int z = 0; z < 7; z++)
			generateChunk(x, z, worldType);
}

Block World::getBlock(int x, int y, int z) const {
	return m_chunks[x / Chunk::CHUNK_SIZE][z / Chunk::CHUNK_SIZE]
		.getBlock(x % Chunk::CHUNK_SIZE, y, z % Chunk::CHUNK_SIZE);
}

void World::setBlock(int x, int y, int z, Block block) {
	m_chunks[x / Chunk::CHUNK_SIZE][z / Chunk::CHUNK_SIZE]
		.setBlock(x % Chunk::CHUNK_SIZE, y, z % Chunk::CHUNK_SIZE, block);
}