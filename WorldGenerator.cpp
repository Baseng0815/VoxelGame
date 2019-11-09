#include "pch.h"

// in der .cpp kann man einbinden, was man will, da entsteht kein Problem
#include "World.h"
#include "WorldGenerator.h"

void WorldGenerator::init(WorldType worldType)
{
	m_type = worldType;
	map = new MapGenerator();

	if (m_type == WORLD_NORMAL)
		map->init(3, 10);
}

void WorldGenerator::generate(World* world, int width, int height)
{
	if (m_type == WORLD_FLAT) {
		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++) {
				world->m_chunks[x][y].init(glm::vec3(x * Chunk::CHUNK_SIZE, 0, y * Chunk::CHUNK_SIZE));
				for (int cy = 0; cy < 4; cy++) {
					Block block;

					if (cy == 0) block = Block(BLOCK_COBBLESTONE);
					if (cy == 3) block = Block(BLOCK_GRASS);
					else block = Block(BLOCK_DIRT);

					for (int cx = 0; cx < Chunk::CHUNK_SIZE; cx++)
						for (int cz = 0; cz < Chunk::CHUNK_SIZE; cz++) {
							world->m_chunks[x][y].setBlock(cx, cy, cz, block);
						}
				}
				world->m_chunks[x][y].updateMesh();
			}
	}
	else if (m_type == WORLD_NORMAL) {	
		int** terrain = map->generateMap(world, width, height);

		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++) {
				world->m_chunks[x][y].init(glm::vec3(x * Chunk::CHUNK_SIZE, 0, y * Chunk::CHUNK_SIZE));

				for (int cx = 0; cx < Chunk::CHUNK_SIZE; cx++)
					for (int cz = 0; cz < Chunk::CHUNK_SIZE; cz++) {
						for (int cy = 0; cy < terrain[x * Chunk::CHUNK_SIZE + cx][y * Chunk::CHUNK_SIZE + cz]; cy++) {
							world->m_chunks[x][y].setBlock(cx, cy, cz, Block(BLOCK_DIRT));
						}
					}

				world->m_chunks[x][y].updateMesh();
			}
	}
}
