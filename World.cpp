#include "pch.h"

#include "World.h"

bool World::checkBounds(int x, int z) const {
	return !(x < 0 || x > 6 || z < 0 || z > 6);
}

Chunk World::generateChunk(int x, int z, WorldType type) {
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
	return chunk;
}

void World::init(WorldType worldType) {
	for (int x = 0; x < 7; x++)
		for (int z = 0; z < 7; z++)
			m_chunks[x][z] = generateChunk(x, z, worldType);
}

Chunk World::getChunk(int x, int z) const {
	if (checkBounds(x, z))
		return m_chunks[x][z];
	else
		throw std::out_of_range("Coordinates out of range.");
}