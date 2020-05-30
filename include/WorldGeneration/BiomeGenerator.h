#pragma once
#include "Biome.h"
#include <noise/noise.h>
#include <glm/glm.hpp>
#include "Gradient.h"

using namespace noise::module;

class BiomeGenerator {
private:
	Perlin perlin; 	
	Perlin land;

	Const desert;
	Const flat;
	Const ocean;
	Const beach;

	Select desertSelect;
	Select oceanSelect;
	Select biomeSelector;

	

public:
	BiomeGenerator();

	void generateBiomes(glm::ivec2 chunkPos, BiomeID** biomeMap) const;
	//bool needsInterpolation(glm::ivec2 chunk) const;
};