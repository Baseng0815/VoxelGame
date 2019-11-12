#include "pch.h"

// in der .cpp kann man einbinden, was man will, da entsteht kein Problem
#include "World.h"
#include "WorldGenerator.h"
#include "Definitions.h"

void WorldGenerator::init(WorldType worldType)
{
	m_type = worldType;
	map = new MapGenerator();
}

void WorldGenerator::generateTerrain(World* world)
{
	if (m_type == WORLD_FLAT) {
		for (int x = 0; x < 5; x++)
			for (int y = 0; y < 5; y++) {
				Chunk* c = world->createChunk(x, y);
				for (int cy = 0; cy < 4; cy++) {
					Block block;

					if (cy == 0) block = Block(BlockType::BLOCK_STONE_COBBLE);
					if (cy == 3) block = Block(BlockType::BLOCK_GRASS);
					else block = Block(BlockType::BLOCK_DIRT);

					for (int cx = 0; cx < Chunk::CHUNK_SIZE; cx++)
						for (int cz = 0; cz < Chunk::CHUNK_SIZE; cz++) {
							c->setBlock(cx, cy, cz, block);
						}
				}
				c->updateMesh();
			}
	}
	else if (m_type == WORLD_NORMAL) {
		map->generateSeed();

		for (int x = 0; x < 10; x++)
			for (int y = 0; y < 10; y++) {
				Chunk* c = world->createChunk(x, y);
				int** cMap = map->generateChunkMap(x, y);

				for (int cx = 0; cx < Chunk::CHUNK_SIZE; cx++) 
					for (int cz = 0; cz < Chunk::CHUNK_SIZE; cz++) {
						int terrainHeight = cMap[cx][cz];
						for (int cy = 0; cy < terrainHeight; cy++) {
							Block block;
							if (cy < terrainHeight - 5)
								block = Block(BlockType::BLOCK_STONE);
							else if (cy < terrainHeight - 1)
								block = Block(BlockType::BLOCK_DIRT);
							else
								block = Block(BlockType::BLOCK_GRASS);

							c->setBlock(cx, cy, cz, block);
						}
					}
				
				c->updateMesh();
				for (int i = 0; i < Chunk::CHUNK_SIZE; i++) {
					delete[] cMap[i];
				}
				delete[] cMap;
			}
	}
}

void WorldGenerator::generateOres(World* world) {

}
