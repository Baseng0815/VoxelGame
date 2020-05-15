#pragma once
#include "Terrain.h"
#include <noise/noise.h>

using namespace noise;
using namespace noise::module;

class FlatTerrain : public Terrain {
private:
	Perlin perlinNoise;
	Select terrainSelector;
	Billow baseFlatTerrain;
	ScaleBias flatTerrain;	

public:
	FlatTerrain();	

	void getBlocks(glm::vec2 chunk, int cx, int cz, Block*** blocks) override;
};
