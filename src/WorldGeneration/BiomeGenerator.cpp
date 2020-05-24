#include "../../include/WorldGeneration/BiomeGenerator.h"
#include "../../include/Configuration.h"

BiomeGenerator::BiomeGenerator() {	
	perlin.SetFrequency(1 / 400.0f);
	perlin.SetNoiseQuality(noise::NoiseQuality::QUALITY_FAST);
	perlin.SetOctaveCount(3);
	perlin.SetPersistence(0.25f);	
}


bool BiomeGenerator::generateBiomes(glm::vec2 chunk, BiomeID** biomeMap) {
	bool needsTerrainInterpolation = false;
	for (int cx = 0; cx < CHUNK_SIZE; cx++)
		for (int cz = 0; cz < CHUNK_SIZE; cz++) {
			double value = perlin.GetValue(chunk.x * CHUNK_SIZE + cx, chunk.y * CHUNK_SIZE + cz, 0);
			if (value < 0.1f) {
				biomeMap[cx][cz] = BiomeID::BIOME_FLAT_TERRAIN;

				if (!needsTerrainInterpolation && biomeMap[0][0] == BiomeID::BIOME_DESERT)
					needsTerrainInterpolation = true;
			}
			else {
				biomeMap[cx][cz] = BiomeID::BIOME_DESERT;

				if (!needsTerrainInterpolation && biomeMap[0][0] == BiomeID::BIOME_FLAT_TERRAIN)
					needsTerrainInterpolation = true;
			}
		}

	return needsTerrainInterpolation;
}

const noise::module::Module& BiomeGenerator::getBiomeNoise() const {
	return perlin;
}