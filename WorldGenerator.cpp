#include "pch.h"

// in der .cpp kann man einbinden, was man will, da entsteht kein Problem
#include "World.h"
#include "WorldGenerator.h"
#include "Definitions.h"
#include "noiseutils.h"

using namespace noise::utils;
using namespace noise::model;
using namespace noise::module;

void WorldGenerator::init(WorldType worldType) {
	m_type = worldType;

	perlinNoise.SetSeed(rand());
	perlinNoise.SetOctaveCount(8);	
	perlinNoise.SetPersistence(1 / 64.0f);
}

void WorldGenerator::generateTerrain(glm::vec2 position, Chunk* chunk) const {
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane planeBuilder = NoiseMapBuilderPlane();

	planeBuilder.SetSourceModule(perlinNoise);
	planeBuilder.SetDestNoiseMap(heightMap);
	planeBuilder.SetDestSize(Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE);
	planeBuilder.SetBounds(position.x, position.x + 1, position.y, position.y + 1);

	planeBuilder.Build();	
	

	for (int cx = 0; cx < Chunk::CHUNK_SIZE; cx++)
		for (int cz = 0; cz < Chunk::CHUNK_SIZE; cz++) {
			int height = heightMap.GetValue(cx, cz) * (float)3 + 40;
			for (int cy = 0; cy < height; cy++) {
				Block block;
				if (cy < height - 5)
					block = Block(BlockType::BLOCK_STONE);
				else if (cy < height - 1)
					block = Block(BlockType::BLOCK_DIRT);
				else
					block = Block(BlockType::BLOCK_GRASS);

				chunk->setBlock(cx, cy, cz, block);
			}
		}

	chunk->updateVertices();
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

void WorldGenerator::generateChunk(glm::vec2 position, Chunk* chunk) const {
	generateTerrain(position, chunk);
}