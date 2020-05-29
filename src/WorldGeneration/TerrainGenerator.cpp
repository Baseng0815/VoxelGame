#include "../../include/WorldGeneration/TerrainGenerator.h"
#include "../../include/WorldGeneration/WorldGenerator.h"
#include "../../include/WorldGeneration/TerrainFlat.h"
#include "../../include/WorldGeneration/TerrainDesert.h"
#include "../../include/WorldGeneration/TerrainOcean.h"

#include "../../include/Configuration.h"
#include "../../include/Block.h"

TerrainGenerator::TerrainGenerator() {
	m_terrains.push_back({ BiomeID::BIOME_FLAT, new Terrain(0, 0) });
	m_terrains.push_back({ BiomeID::BIOME_FLAT_TERRAIN, new TerrainFlat() });
	m_terrains.push_back({ BiomeID::BIOME_DESERT, new TerrainDesert() });
	m_terrains.push_back({ BiomeID::BIOME_OCEAN, new TerrainOcean() });
	
}

TerrainGenerator::~TerrainGenerator() {
	for (auto biome : m_terrains)
		delete biome.generator;
}

Biome TerrainGenerator::getTerrain(BiomeID id) const {
	bool found = false;

	auto it = m_terrains.begin();
	do {
		if ((*it).id == id) {
			found = true;
		}
		else it++;
	} while (!found && it != m_terrains.end());

	if (it != m_terrains.end()) {
		Biome biome = *it;
		return biome;
	}

	return m_terrains[0];
}

void TerrainGenerator::generate(glm::vec2 position, BiomeID** biomes, Block*** blocks, bool needsInterpolation) {
	if (!needsInterpolation)
		for (int cx = 0; cx < CHUNK_SIZE; cx++)
			for (int cz = 0; cz < CHUNK_SIZE; cz++) {
				BiomeID biome = biomes[cx][cz];

				getTerrain(biome).generator->getBlocks(position, cx, cz, blocks);

		}
	else {
		// create chunk corners
		int** heightMap = new int*[CHUNK_SIZE];
		for(int i = 0; i < CHUNK_SIZE; i++)
			heightMap[i] = new int[CHUNK_SIZE];

		heightMap[0][0] = getTerrain(biomes[0][0]).generator->getHeight(position, 0, 0);

		heightMap[0][CHUNK_SIZE - 1] = getTerrain(biomes[0][CHUNK_SIZE - 1])
			.generator->getHeight(position, 0, CHUNK_SIZE - 1);

		heightMap[CHUNK_SIZE - 1][0] = getTerrain(biomes[CHUNK_SIZE - 1][0])
			.generator->getHeight(position, CHUNK_SIZE - 1, 0);

		heightMap[CHUNK_SIZE - 1][CHUNK_SIZE - 1] = getTerrain(biomes[CHUNK_SIZE - 1][CHUNK_SIZE - 1])
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

				getTerrain(biome).generator->getBlocks(position, cx, cz, blocks, heightMap[cx][cz]);
			}
	}		
}