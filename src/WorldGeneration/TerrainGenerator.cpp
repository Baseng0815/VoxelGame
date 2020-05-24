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
		for (int cx = 0; cx < CHUNK_SIZE; cx++)
			for (int cz = 0; cz < CHUNK_SIZE; cz++) {
				BiomeID biome = biomes[cx][cz];

				m_biomes[(int)biome].generator->getBlocks(position, cx, cz, blocks);

		}
	else {
		// create chunk corners
		int** heightMap = new int*[CHUNK_SIZE];
		for(int i = 0; i < CHUNK_SIZE; i++)
			heightMap[i] = new int[CHUNK_SIZE];

		heightMap[0][0] = m_biomes[biomes[0][0]].generator->getHeight(position, 0, 0);

		heightMap[0][CHUNK_SIZE - 1] = m_biomes[biomes[0][CHUNK_SIZE - 1]]
			.generator->getHeight(position, 0, CHUNK_SIZE - 1);

		heightMap[CHUNK_SIZE - 1][0] = m_biomes[biomes[CHUNK_SIZE - 1][0]]
			.generator->getHeight(position, CHUNK_SIZE - 1, 0);

		heightMap[CHUNK_SIZE - 1][CHUNK_SIZE - 1] = m_biomes[biomes[CHUNK_SIZE - 1][CHUNK_SIZE - 1]]
			.generator->getHeight(position, CHUNK_SIZE - 1, CHUNK_SIZE - 1);

		// interpolate chunk height
		int q11 = heightMap[0][0];
		int q12 = heightMap[0][CHUNK_SIZE - 1];
		int q21 = heightMap[CHUNK_SIZE - 1][0];
		int q22 = heightMap[CHUNK_SIZE - 1][CHUNK_SIZE - 1];

		for(int cx = 0; cx < CHUNK_SIZE; cx++) {
			float r1 = (float)(CHUNK_SIZE - cx) / CHUNK_SIZE * q11 + (float)cx / CHUNK_SIZE * q21;
			float r2 = (float)(CHUNK_SIZE - cx) / CHUNK_SIZE * q12 + (float)cx / CHUNK_SIZE * q22;
			for(int cz= 0; cz < CHUNK_SIZE; cz++) {
				float value = (float)(CHUNK_SIZE - cz) / CHUNK_SIZE * r1 + (float)cz / CHUNK_SIZE * r2;

				heightMap[cx][cz] = round(value);
			}
		}

		for(int cx = 0; cx < CHUNK_SIZE; cx++)
			for(int cz = 0; cz < CHUNK_SIZE; cz++){
				BiomeID biome = biomes[cx][cz];

				m_biomes[(int)biome].generator->getBlocks(position, cx, cz, blocks, heightMap[cx][cz]);
			}
	}		
}