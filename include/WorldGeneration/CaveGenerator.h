#pragma once
#include <glm/glm.hpp>
#include <noise/noise.h>

struct Block;
using namespace noise::module;

class CaveGenerator {
  public:
    RidgedMulti caveNoise;
    float threshold = 0.9f;

    CaveGenerator();

    void generateChunk(glm::vec2 chunkPos, Block*** blocks) const;
};
