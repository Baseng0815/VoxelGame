#pragma once
#include "Terrain.h"
#include <noise/noise.h>
#include <glm/glm.hpp>

using namespace noise;
using namespace noise::module;

struct Block;

class TerrainDesert : public Terrain {
private:
    Billow baseTerrainDesert;
    ScaleBias terrainDesert;
    
public:
    TerrainDesert();

    void getBlocks(glm::vec2 chunk, int cx, int cz, Block*** blocks) override;
};
