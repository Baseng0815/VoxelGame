#pragma once
#include "Biome.h"
#include <glm/glm.hpp>

class BiomeGenerator {
public:
	void generateBiomes(glm::vec2 chunkPos, BiomeID** biomeMap);
};