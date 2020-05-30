#pragma once
#include "Terrain.h"
#include <noise/noise.h>

using namespace noise::module;

class TerrainOcean : public Terrain {
private:
	RidgedMulti oceanMoutainsInverted;
	ScaleBias oceanMoutains;
	Multiply oceanTerrain;

public:
	TerrainOcean();

	void getBlocks(int cx, int cz, Block*** blocks, int terrainHeight) override;
	int getHeight(glm::ivec2 chunk, int cx, int cz) const override;
};