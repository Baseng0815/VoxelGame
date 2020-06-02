#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "noise/noise.h"

using namespace noise;
struct Block;

class CaveGenerator {
private:
    module::Billow caveNoise1;
    module::Perlin caveNoise2;
    module::Multiply caveNoiseSum;
    module::Const cave;
    module::Const solid;
    module::ScalePoint scale;

    module::Select caveNoise;    

public:
    CaveGenerator();

    CaveGenerator& operator=(const CaveGenerator&);

    void generate(glm::ivec2 position, Block*** blocks);
};

