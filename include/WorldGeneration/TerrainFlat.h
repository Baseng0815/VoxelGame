#pragma once
#include "Terrain.h"
#include <noise/noise.h>

using namespace noise;
using namespace noise::module;

class TerrainFlat : public Terrain {
private:	
	Billow baseFlatTerrain;
	ScaleBias flatTerrain;	

public:
	TerrainFlat();	

	void getBlocks(glm::vec2 chunk, int cx, int cz, Block*** blocks, int terrainHeight = -1) override;
	int getHeight(glm::vec2 chunk, int cx, int cz) const override;
};
