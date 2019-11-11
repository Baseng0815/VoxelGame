#include "pch.h"

// in der .cpp kann man einbinden, was man will, da entsteht kein Problem
#include "World.h"
#include "WorldGenerator.h"
#include "Definitions.h"

void WorldGenerator::init(WorldType worldType)
{
	m_type = worldType;
	map = new MapGenerator(5);
}

void WorldGenerator::generateTerrain(World* world)
{
	if (m_type == WORLD_FLAT) {
		for (int x = 0; x < 7; x++)
			for (int y = 0; y < 7; y++) {
				world->m_chunks[x][y].init(glm::vec3(x * Chunk::CHUNK_SIZE, 0, y * Chunk::CHUNK_SIZE));
				for (int cy = 0; cy < 4; cy++) {
					Block block;

					if (cy == 0) block = Block(BlockType::BLOCK_STONE);
					if (cy == 3) block = Block(BlockType::BLOCK_GRASS);
					else block = Block(BlockType::BLOCK_DIRT);

					for (int cx = 0; cx < Chunk::CHUNK_SIZE; cx++)
						for (int cz = 0; cz < Chunk::CHUNK_SIZE; cz++) {
							world->m_chunks[x][y].setBlock(cx, cy, cz, block);
						}
				}
				world->m_chunks[x][y].updateMesh();
			}
	}
	else if (m_type == WORLD_NORMAL) {
		map->generateSeed();
		int** terrain = map->generateMap(world);

		for (int x = 0; x < 5; x++)
			for (int y = 0; y < 5; y++) {
				world->m_chunks[x][y].init(glm::vec3(x * Chunk::CHUNK_SIZE, 0, y * Chunk::CHUNK_SIZE));

				for (int cx = 0; cx < Chunk::CHUNK_SIZE; cx++)
					for (int cz = 0; cz < Chunk::CHUNK_SIZE; cz++) {
						int terrainHeight = terrain[x * Chunk::CHUNK_SIZE + cx][y * Chunk::CHUNK_SIZE + cz];
						for (int cy = 0; cy < terrainHeight; cy++) {
							if (cy > terrainHeight - 5)
								world->m_chunks[x][y].setBlock(cx, cy, cz, Block(BlockType::BLOCK_DIRT));
							else
								world->m_chunks[x][y].setBlock(cx, cy, cz, Block(BlockType::BLOCK_STONE));
						}
						world->m_chunks[x][y].setBlock(cx, terrainHeight, cz, Block(BlockType::BLOCK_GRASS));
					}

				world->m_chunks[x][y].updateMesh();
			}

		for(int i = 0; i < 5; i++)
			delete[] terrain[i];

		delete[] terrain;
	}
}

void WorldGenerator::generateOres(World* world) {
	
}
