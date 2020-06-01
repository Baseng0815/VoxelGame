// in der .cpp kann man einbinden, was man will, da entsteht kein Problem
#include "../../include/WorldGeneration/WorldGenerator.h"
#include "../../include/WorldGeneration/noiseutils.h"
#include "../../include/WorldGeneration/Biome.h"
#include "../../include/Block.h"
#include "../../include/BlockData.h"
#include "../../include/Configuration.h"
#include "../../include/Utility.h"

using namespace noise::utils;
using namespace noise::model;

WorldGenerator::WorldGenerator(WorldType worldType) {
	m_type = worldType;	
}

WorldGenerator& WorldGenerator::operator=(const WorldGenerator& generator) {
	m_caveGenerator = generator.m_caveGenerator;

	return *this;
}

void WorldGenerator::generateOres(BiomeID** biomes, Block*** blocks) const {
	for (BlockType block = BlockType::BLOCK_ORE_GOLD; block <= BlockType::BLOCK_ORE_COAL; ) {
		BlockData blockData = Configuration::getBlockData(block);

		for (int c = 0; c < blockData.oreData.generationCounts; c++) {
			int x = rand() % CHUNK_SIZE;
			int y = rand() % (blockData.oreData.maxHeight - blockData.oreData.minHeight) + blockData.oreData.minHeight;
			int z = rand() % CHUNK_SIZE;	
			glm::ivec3 orePos = glm::ivec3(x, y, z);

			double a = pow(3 * blockData.oreData.size / (16 * PI), 1 / 3.0);

			//		x^2		y^2		z^2
			// 1 = ----- + ----- + -----
			//		a^2		a^2		4a^2

			for(int x1 = -a; x1 <= a; x1++)
				for (int z1 = -2 * a; z1 <= 2 * a; z1++) {
					double fSquared = pow(a, 2) - pow(x1, 2) - (pow(z1, 2) / pow(a, 2));

					if (fSquared >= 0) {
						double ymin = -pow(fSquared, 0.5);
						double ymax = -ymin;

						for (int y1 = ymin; y1 <= ymax; y1++) {
							glm::ivec3 pos = orePos + glm::ivec3(x1, y1, z1);							

							if (blocks[pos.x][pos.y][pos.z].type == BlockType::BLOCK_STONE) {
								blocks[pos.x][pos.y][pos.z] = Block(block);
							}
						}
					}
				}
		}
		
		block = (BlockType)((int)block + 1);
	}
}


void WorldGenerator::generate(glm::ivec2 position, BiomeID** biomes, Block*** blocks) {
	int** heightMap = new int*[CHUNK_SIZE];
	for(int i = 0; i < CHUNK_SIZE; i++) heightMap[i] = new int[CHUNK_SIZE];

	m_heightGenerator.generateChunkHeight(position, heightMap, biomes);
	m_terrainGenerator.createBlocks(blocks, heightMap, biomes);
	//generateOres(biomes, blocks);

	//m_caveGenerator.generate(position, blocks);
}
