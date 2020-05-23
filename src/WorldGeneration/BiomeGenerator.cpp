#include "../../include/WorldGeneration/BiomeGenerator.h"
#include "../../include/Configuration.h"

BiomeGenerator::BiomeGenerator() {
	//perlin.SetFrequency(1 / 16348.0f);
	perlin.SetFrequency(1 / 200.0f);
	perlin.SetNoiseQuality(noise::NoiseQuality::QUALITY_FAST);
	perlin.SetOctaveCount(3);

	biomeTurb.SetSourceModule(0, perlin);
}


// TODO overwork biome borders
bool BiomeGenerator::generateBiomes(glm::vec2 chunk, BiomeID** biomeMap) {
	bool needsTerrainInterpolation = false;
	for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++)
		for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
			double value = biomeTurb.GetValue(chunk.x * Configuration::CHUNK_SIZE + cx, chunk.y * Configuration::CHUNK_SIZE + cz, 0);
			if (value < 0.3f) {
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
	return biomeTurb;
}