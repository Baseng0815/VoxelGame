#pragma once
#include "Terrain.h"
#include <noise/noise.h>

using namespace noise;
using namespace noise::module;

class TerrainFlat : public Terrain {
private:		
	ScaleBias flatTerrain;	

public:
	TerrainFlat();	

	void getBlocks(int cx, int cz, Block*** blocks, int terrainHeight) override;
	int getHeight(glm::ivec2 chunk, int cx, int cz) const override;
};
