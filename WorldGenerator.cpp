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

void WorldGenerator::generateTerrain(World& world)
{
	if (m_type == WorldType::WORLD_FLAT) {
		for (int x = 0; x < 5; x++)
			for (int y = 0; y < 5; y++) {
				Chunk c;
				c.init(glm::vec3(x * Chunk::CHUNK_SIZE, 0, y * Chunk::CHUNK_SIZE));
				for (int cy = 0; cy < 4; cy++) {
					Block block;

					if (cy == 0) block = Block(BlockType::BLOCK_STONE_COBBLE);
					if (cy == 3) block = Block(BlockType::BLOCK_GRASS);
					else block = Block(BlockType::BLOCK_DIRT);

					for (int cx = 0; cx < Chunk::CHUNK_SIZE; cx++)
						for (int cz = 0; cz < Chunk::CHUNK_SIZE; cz++) {
							c.setBlock(cx, cy, cz, block);
						}
				}
				
				world.m_chunks[x][y] = c;
			}
	}
	else if (m_type == WorldType::WORLD_NORMAL) {
		map->generateSeed();

		for (int x = 0; x < 5; x++)
			for (int y = 0; y < 5; y++) {
				Chunk c;
				c.init(glm::vec3(x * Chunk::CHUNK_SIZE, 0, y * Chunk::CHUNK_SIZE));
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

							c.setBlock(cx, cy, cz, block);
						}
					}

				generateOres(world, c);

				world.m_chunks[x][y] = c;

				for (int i = 0; i < Chunk::CHUNK_SIZE; i++) {
					delete[] cMap[i];
				}
				delete[] cMap;
			}
	}
}

void WorldGenerator::generateOres(const World& world, Chunk& chunk) {
	for (int i = 0; i < Definitions::BLOCK_DATA.size(); i++) {
		BlockData blockData = Definitions::BLOCK_DATA[i];
		if (blockData.oreData.generationCounts > 0) {
			for (int c = 0; c < blockData.oreData.generationCounts; c++) {
				int y = rand() % (blockData.oreData.maxHeight - blockData.oreData.minHeight) + blockData.oreData.minHeight;

				int x = rand() % Chunk::CHUNK_SIZE;
				int z = rand() % Chunk::CHUNK_SIZE;

				int rotation = rand() % 4;
				int height = 2;
				int width = sqrt(blockData.oreData.size / height);
				// set blocks
				for (int ox = 0; ox < width; ox++)
					for (int oy = 0; oy < height; oy++)
						for (int oz = 0; oz < width; oz++) {
							switch (rotation)
							{
							case 0:
								if (x + ox < Chunk::CHUNK_SIZE && z + oz < Chunk::CHUNK_SIZE)
									if (chunk.getBlock(x + ox, y + oy, z + oz).type == BlockType::BLOCK_STONE)
										chunk.setBlock(x + ox, y + oy, z + oz, (BlockType)i);
								break;
							case 1:
								if (x - ox >= 0 && z + oz < Chunk::CHUNK_SIZE)
									if (chunk.getBlock(x - ox, y + oy, z + oz).type == BlockType::BLOCK_STONE)
										chunk.setBlock(x - ox, y + oy, z + oz, (BlockType)i);
								break;
							case 2:
								if (z - oz >= 0 && x + ox < Chunk::CHUNK_SIZE)
									if (chunk.getBlock(x + ox, y + oy, z - oz).type == BlockType::BLOCK_STONE)
										chunk.setBlock(x + ox, y + oy, z - oz, (BlockType)i);

								break;
							case 3:
								if (x - ox >= 0 && z - oz >= 0)
									if (chunk.getBlock(x - ox, y + oy, z - oz).type == BlockType::BLOCK_STONE)
										chunk.setBlock(x - ox, y + oy, z - oz, (BlockType)i);

								break;
							}
						}
			}
		}
	}
}

void WorldGenerator::generate(World& world) {
	generateTerrain(world);
}