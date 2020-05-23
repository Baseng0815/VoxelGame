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

void TerrainGenerator::generate(glm::vec2 position, BiomeID** biomes, Block*** blocks, bool needsInterpolation) {
	if (!needsInterpolation)
		for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++)
			for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
				BiomeID biome = biomes[cx][cz];

				m_biomes[(int)biome].generator->getBlocks(position, cx, cz, blocks);

			}
	else {
		// create chunk corners
		int** heightMap = new int*[Configuration::CHUNK_SIZE];
		for(int i = 0; i < Configuration::CHUNK_SIZE; i++)
			heightMap[i] = new int[Configuration::CHUNK_SIZE];

		heightMap[0][0] = m_biomes[biomes[0][0]].generator->getHeight(position, 0, 0);

		heightMap[0][Configuration::CHUNK_SIZE - 1] = m_biomes[biomes[0][Configuration::CHUNK_SIZE - 1]]
			.generator->getHeight(position, 0, Configuration::CHUNK_SIZE - 1);

		heightMap[Configuration::CHUNK_SIZE - 1][0] = m_biomes[biomes[Configuration::CHUNK_SIZE - 1][0]]
			.generator->getHeight(position, Configuration::CHUNK_SIZE - 1, 0);

		heightMap[Configuration::CHUNK_SIZE - 1][Configuration::CHUNK_SIZE - 1] = m_biomes[biomes[Configuration::CHUNK_SIZE - 1][Configuration::CHUNK_SIZE - 1]]
			.generator->getHeight(position, Configuration::CHUNK_SIZE - 1, Configuration::CHUNK_SIZE - 1);

		// interpolate chunk height
		float dx0 = (float)(heightMap[Configuration::CHUNK_SIZE - 1][0] - heightMap[0][0]) / Configuration::CHUNK_SIZE;
		float dx1 = (float)(heightMap[Configuration::CHUNK_SIZE - 1][Configuration::CHUNK_SIZE - 1] - heightMap[0][Configuration::CHUNK_SIZE - 1]) / Configuration::CHUNK_SIZE;

		for(int cx = 0; cx < Configuration::CHUNK_SIZE; cx++) {
			float dz = (dx0 - dx1) * cx / Configuration::CHUNK_SIZE;
			for(int cz= 0; cz < Configuration::CHUNK_SIZE; cz++) {
				heightMap[cx][cz] = heightMap[0][0] + dx0 * cx + dz * cz;
			}
		}

		for(int cx = 0; cx < Configuration::CHUNK_SIZE; cx++)
			for(int cz = 0; cz < Configuration::CHUNK_SIZE; cz++){
				BiomeID biome = biomes[cx][cz];

				m_biomes[(int)biome].generator->getBlocks(position, cx, cz, blocks, heightMap[cx][cz]);
			}
	}		
}