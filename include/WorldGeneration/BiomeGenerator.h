#pragma once
#include "Biome.h"
#include <noise/noise.h>
#include <glm/glm.hpp>

class BiomeGenerator {
private:
	noise::module::Perlin perlin;

public:
	BiomeGenerator();

	void generateBiomes(glm::vec2 chunkPos, BiomeID** biomeMap);
};