// in der .cpp kann man einbinden, was man will, da entsteht kein Problem
//#include "../../include/WorldGeneration/WorldGenerator.hpp"
#include "../../include/WorldGeneration/noiseutils.hpp"
#include "../../include/Configuration.hpp"
#include "../../include/Utility.hpp"
#include "../../include/Systems/ChunkCreateSystem.hpp"

#include "../../include/GameData/GameData.hpp"

#include <iostream>

using namespace noise::utils;
using namespace noise::model;

WorldGenerator::WorldGenerator(WorldType type) {
    m_type = type;
}

void WorldGenerator::generateOres(BiomeId** biomes, Block*** blocks) const {
	for (BlockId block = BlockId::BLOCK_ORE_GOLD; block <= BlockId::BLOCK_ORE_COAL; ) {
		BlockTemplate blockTemplate = GameData::getBlockTemplate(block);

		for (int c = 0; c < blockTemplate.oreData.generationCounts; c++) {
			int x = rand() % Configuration::CHUNK_SIZE;
			int y = rand() % (blockTemplate.oreData.maxHeight - blockTemplate.oreData.minHeight) + blockTemplate.oreData.minHeight;
			int z = rand() % Configuration::CHUNK_SIZE;
			glm::vec3 orePos = glm::vec3(x, y, z);

			double a = pow(3 * blockTemplate.oreData.size / (16 * PI), 1 / 3.0);

			//		x^2		y^2		z^2
			// 1 = ----- + ----- + -----
			//		a^2		a^2		4a^2

			for (int x1 = -a; x1 <= a; x1++)
				for (int z1 = -2 * a; z1 <= 2 * a; z1++) {
					double fSquared = pow(a, 2) - pow(x1, 2) - (pow(z1, 2) / pow(a, 2));

					if (fSquared >= 0) {
						double ymin = -pow(fSquared, 0.5);
						double ymax = -ymin;

						for (int y1 = ymin; y1 <= ymax; y1++) {
							glm::vec3 pos = orePos + glm::vec3(x1, y1, z1);

							if (pos.x < 0 || pos.x >= Configuration::CHUNK_SIZE || pos.y < 0 || pos.y >= Configuration::CHUNK_HEIGHT || pos.z < 0 || pos.z >= Configuration::CHUNK_SIZE)
								continue;

							if (blocks[(int)pos.x][(int)pos.y][(int)pos.z].type == BlockId::BLOCK_STONE) {
								blocks[(int)pos.x][(int)pos.y][(int)pos.z] = Block {block};
							}
						}
					}
				}
		}

		block = (BlockId)((int)block + 1);
	}
}

void WorldGenerator::generate(glm::vec2 position, GenerationData* data) {
	int** heightMap = new int* [Configuration::CHUNK_SIZE];
	for (int i = 0; i < Configuration::CHUNK_SIZE; i++) heightMap[i] = new int[Configuration::CHUNK_SIZE];

	m_heightGenerator.generateChunkHeight(position, heightMap, data->biomes);
	m_terrainGenerator.createBlocks(data->blocks, heightMap, data->biomes);
	generateOres(data->biomes, data->blocks);

	m_caveGenerator.generateChunk(position, data->blocks);	
}

int WorldGenerator::getSurfaceHeight(glm::vec2 chunk, int x, int z) const {
	return m_heightGenerator.getHeight(chunk, x, z);
}