#pragma once
#include "Terrain.h"
#include <noise/noise.h>
#include <glm/glm.hpp>

using namespace noise;
using namespace noise::module;

struct Block;

class TerrainDesert : public Terrain {
private:    
    ScaleBias terrainDesert;
    
public:
    TerrainDesert();

    void getBlocks(glm::vec2 chunk, int cx, int cz, Block*** blocks, int terrainHeight = -1) override;
    int getHeight(glm::vec2 chunk, int cx, int cz) const override;
};
