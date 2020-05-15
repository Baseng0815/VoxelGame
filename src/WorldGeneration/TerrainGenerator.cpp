#include "../../include/WorldGeneration/TerrainGenerator.h"
#include "../../include/WorldGeneration/WorldGenerator.h"
#include "../../include/WorldGeneration/FlatTerrain.h"

#include "../../include/Configuration.h"
#include "../../include/Block.h"

TerrainGenerator::TerrainGenerator() {
	Terrain* terrain = new Terrain(0, 0);
	FlatTerrain* flat = new FlatTerrain();

	m_biomes.push_back({ BiomeID::BIOME_FLAT, terrain });
	m_biomes.push_back({ BiomeID::BIOME_FLAT_TERRAIN, flat });
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