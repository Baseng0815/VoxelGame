#include "../../include/WorldGeneration/BiomeGenerator.h"
#include "../../include/Configuration.h"

void BiomeGenerator::generateBiomes(glm::vec2 chunk, BiomeID** biomeMap) {
	for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++)
		for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++)
			biomeMap[cx][cz] = BiomeID::BIOME_FLAT_TERRAIN;
}