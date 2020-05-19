#include "../../include/WorldGeneration/TerrainGenerator.h"
#include "../../include/WorldGeneration/WorldGenerator.h"
#include "../../include/WorldGeneration/TerrainFlat.h"
#include "../../include/WorldGeneration/TerrainDesert.h"

#include "../../include/Configuration.h"
#include "../../include/Block.h"

TerrainGenerator::TerrainGenerator() {	
	m_biomes.push_back({ BiomeID::BIOME_FLAT, new Terrain(0, 0) });
	m_biomes.push_back({ BiomeID::BIOME_FLAT_TERRAIN, new TerrainFlat() });
	m_biomes.push_back({ BiomeID::BIOME_DESERT, new TerrainDesert() });
}

TerrainGenerator::~TerrainGenerator() {
	for (auto biome : m_biomes)
		delete biome.generator;
}

void TerrainGenerator::generate(glm::vec2 position, BiomeID** biomes, Block*** blocks) {
	for(int cx = 0; cx < Configuration::CHUNK_SIZE; cx++)
		for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
			BiomeID biome = biomes[cx][cz];	

			m_biomes[(int)biome].generator->getBlocks(position, cx, cz, blocks);
		}
}