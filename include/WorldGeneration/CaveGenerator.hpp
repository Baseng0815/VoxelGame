#pragma once
#include <glm/glm.hpp>
#include <noise/noise.h>

#include "GenerationData.hpp"

struct Block;
using namespace noise::module;

class CaveGenerator {
  public:
    RidgedMulti caveNoise;
    float threshold = 0.9f;

    CaveGenerator();

    void generateChunk(glm::vec2 chunkPos, GenerationData* blocks) const;
};
