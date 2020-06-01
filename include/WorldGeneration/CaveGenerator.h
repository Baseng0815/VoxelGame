#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "noise/noise.h"

using namespace noise;
struct Block;

class CaveGenerator {
private:
    module::RidgedMulti caveNoise1;
    module::RidgedMulti caveNoise2;
    module::Multiply caveNoiseProd;
    module::Clamp caveNoiseClamp;

    module::Turbulence caveTurb;    

    module::ScaleBias caveNoise;
    

public:
    CaveGenerator();

    CaveGenerator& operator=(const CaveGenerator&);

    void generate(glm::ivec2 position, Block*** blocks);
};

