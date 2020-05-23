#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Biome.h"

struct Block;

class TerrainGenerator {
private:
	std::vector<Biome> m_biomes;
public:		

	TerrainGenerator();	
	~TerrainGenerator();

	void generate(glm::vec2 chunkPosition, BiomeID** biomes, Block*** blocks, bool needsInterpolation = false);				
};