#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Block.h"
#include "noise/noise.h"

using namespace noise;

class CaveGenerator {
private:
    module::RidgedMulti caveNoise1;
    module::RidgedMulti caveNoise2;
    module::Multiply caveNoiseProd;
    module::ScaleBias caveNoiseScale;

    module::Turbulence caveTurb;
    module::Turbulence turbY;
    module::Turbulence turbZ;

    module::ScaleBias caveNoise;

    float threshold = 0.3f;

public:
    CaveGenerator();

    CaveGenerator& operator=(const CaveGenerator&);

    void generate(glm::vec2 position, Block*** blocks);
};

