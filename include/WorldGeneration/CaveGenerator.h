#pragma once

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <noise/noise.h>

#include "../Utility.h"
#include "PerlinWorm.h"

struct Block;
using namespace noise::module;

class CaveGenerator {
  public:
    RidgedMulti caveNoise;
    float threshold = 0.9f;

    std::vector<PerlinWorm> m_worms;
    std::unordered_map<glm::vec2, std::vector<WormPart>, Utility::HashFunctionVec2> m_cache;

    void createWorm(glm::vec2 chunk);

public:
    CaveGenerator();

    void generate(glm::vec2 position, Block*** blocks);
};
