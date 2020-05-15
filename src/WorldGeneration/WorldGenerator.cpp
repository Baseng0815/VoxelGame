// in der .cpp kann man einbinden, was man will, da entsteht kein Problem
#include "../../include/WorldGeneration/WorldGenerator.h"
#include "../../include/WorldGeneration/noiseutils.h"
#include "../../include/Block.h"
#include "../../include/BlockData.h"
#include "../../include/Configuration.h"

using namespace noise::utils;
using namespace noise::model;
using namespace noise::module;

WorldGenerator::WorldGenerator(WorldType worldType) {
	m_type = worldType;	
}

WorldGenerator& WorldGenerator::operator=(const WorldGenerator& generator) {
	m_caveGenerator = generator.m_caveGenerator;

	return *this;
}

#include <iostream>

void WorldGenerator::generateUnderground(glm::vec2 position, Block*** blocks) {
	if (m_type == WorldType::WORLD_FLAT)
		return;

	//// generate ores
	for (int b = 0; b < (int)BlockType::NUM_BLOCKS; b++) {
		BlockData blockData = Configuration::getBlockData((BlockType)b);
		for (int i = 0; i < blockData.oreData.generationCounts; i++) {
			int x = rand() % 16;
			int y = rand() % (blockData.oreData.maxHeight - blockData.oreData.minHeight) + blockData.oreData.minHeight;
			int z = rand() % 16;

			int size = blockData.oreData.size;
			generateOre(x, y, z, size, (BlockType)b, blocks);
		}
	}

	// caves
	m_caveGenerator.generate(position, blocks);
}

void WorldGenerator::generateOre(int x, int y, int z, int size, BlockType block, Block*** blocks) const {
	for (int i = 0; i < size; i += 4) {
		float x1 = x + i;
		float z1 = z + i;

		for (int k = 0; k < 2; k++)
			for (int j = 0; j < 2; j++)
				if (i + k + j < size) {
					int x2 = x1 + k;
					int y2 = y + j;
					int z2 = z1;

					if (x2 >= 0 && x2 < 16 && y2 >= 0 && y2 < 256 && z2 >= 0 && z2 < 16) {
						if (blocks[x2][y2][z2].type == BlockType::BLOCK_STONE)
							blocks[x2][y2][z2] = Block(block);
					}
				}
	}
}

void WorldGenerator::generate(glm::vec2 position, BiomeID** biomes, Block*** blocks) {
	m_biomeGenerator.generateBiomes(position, biomes);
	m_terrainGenerator.generate(position, biomes, blocks);
	m_caveGenerator.generate(position, blocks);
}
