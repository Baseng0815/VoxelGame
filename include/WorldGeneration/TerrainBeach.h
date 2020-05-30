#pragma once
#include "Terrain.h"

class TerrainBeach : public Terrain {
public:
	TerrainBeach();

	void getBlocks(int cx, int cz, Block*** blocks, int terrainHeight) override;
	int getHeight(glm::ivec2 chunk, int cx, int cz) const override;
};