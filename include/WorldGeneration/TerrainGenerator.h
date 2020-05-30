#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "Biome.h"

struct Block;

class TerrainGenerator {
private:
	std::map<glm::ivec2, int**> m_terrainHeight;
	std::vector<Biome> m_terrains;	

	Biome getTerrain(BiomeID id) const;
public:		

	TerrainGenerator();	
	~TerrainGenerator();

	void generate(glm::ivec2 chunkPosition, BiomeID** biomes, Block*** blocks, int** heightMap);				
	int** getHeightMap(glm::ivec2 chunk, BiomeID** biomes) const;
};