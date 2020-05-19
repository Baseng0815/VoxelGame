#include "../../include/WorldGeneration/BiomeGenerator.h"
#include "../../include/Configuration.h"

BiomeGenerator::BiomeGenerator() {
	//perlin.SetFrequency(1 / 16348.0f);
	perlin.SetFrequency(1 / 200.0f);
	perlin.SetNoiseQuality(noise::NoiseQuality::QUALITY_FAST);
	perlin.SetOctaveCount(1);
}


// TODO overwork biome borders
void BiomeGenerator::generateBiomes(glm::vec2 chunk, BiomeID** biomeMap) {
	for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++)
		for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
			if (perlin.GetValue(chunk.x * Configuration::CHUNK_SIZE + cx, chunk.y * Configuration::CHUNK_SIZE + cz, 0) < 0.1f)
				biomeMap[cx][cz] = BiomeID::BIOME_FLAT_TERRAIN;
			else
				biomeMap[cx][cz] = BiomeID::BIOME_DESERT;
		}
}