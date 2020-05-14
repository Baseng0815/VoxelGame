#pragma once
#include <glm/glm.hpp>

struct Block;

template<BiomeID id>
class TerrainGenerator {
protected:
	virtual void _init();
public:
	virtual void generateTerrain(glm::vec2 chunkPosition, Block*** blocks);	

	TerrainGenerator();
};