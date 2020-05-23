#pragma once
#include "Biome.h"
#include <noise/noise.h>
#include <glm/glm.hpp>

class BiomeGenerator {
private:
	noise::module::Perlin perlin;
	noise::module::Turbulence biomeTurb;

public:
	BiomeGenerator();

	bool generateBiomes(glm::vec2 chunkPos, BiomeID** biomeMap);
	const noise::module::Module& getBiomeNoise() const;
};