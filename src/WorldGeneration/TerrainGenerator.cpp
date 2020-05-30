#include "../../include/WorldGeneration/TerrainGenerator.h"
#include "../../include/WorldGeneration/WorldGenerator.h"
#include "../../include/WorldGeneration/TerrainFlat.h"
#include "../../include/WorldGeneration/TerrainDesert.h"
#include "../../include/WorldGeneration/TerrainOcean.h"
#include "../../include/WorldGeneration/TerrainBeach.h"

#include "../../include/Configuration.h"
#include "../../include/Block.h"

TerrainGenerator::TerrainGenerator() {
	

}

TerrainGenerator::~TerrainGenerator() {
	for (auto biome : m_terrains)
		delete biome.generator;
}

 

void TerrainGenerator::generate(glm::ivec2 position, BiomeID** biomes, Block*** blocks, int** heightMap) {
	for (int cx = 0; cx < CHUNK_SIZE; cx++)
		for (int cz = 0; cz < CHUNK_SIZE; cz++) {
			BiomeID biome = biomes[cx][cz];
			for (int cy = 0; cy < 63; cy++)
				blocks[cx][cy][cz] = Block(BlockType::BLOCK_WATER);
			
			/*int height = 

			getTerrain(biome).generator->getBlocks(position, cx, cz, blocks, heightMap[cx][cz]);*/

		}
}

int** TerrainGenerator::getHeightMap(glm::ivec2 chunk, BiomeID** biomes) const {
	return nullptr;
}