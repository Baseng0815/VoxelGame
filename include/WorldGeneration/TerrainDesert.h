#pragma once
#include "Terrain.h"
#include <noise/noise.h>

using namespace noise;
using namespace noise::module;

class TerrainDesert : public Terrain {
private:
    Perlin perlinNoise;
    
};
