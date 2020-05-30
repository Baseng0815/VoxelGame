#pragma once
#include <glm/glm.hpp>
#include <noise/noise.h>

struct Block;

class Terrain {
protected:
	int minHeight, maxHeight;
	noise::module::Perlin basePerlin;
	noise::module::ScaleBias baseTerrain;

public:		
	Terrain(int minHeight, int maxHeight);

	virtual void getBlocks(int cx, int cz, Block*** blocks, int terrainHeight);
	virtual int getHeight(glm::ivec2 chunk, int cx, int cz) const;	
	int** generateHeightMap(glm::ivec2 chunk) const;
};