#pragma once
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <noise/noise.h>
#include "../Utility.h"
#include "../Configuration.h"
#include "PerlinWorm.h"

class Block;
using namespace noise::module;

typedef std::vector<glm::vec3> Region;

class CaveGenerator {
private:
  Perlin noise;
  int cache[3 * Configuration::CHUNK_SIZE][Configuration::CHUNK_HEIGHT][3 * Configuration::CHUNK_SIZE];

  //void smoothMap();
  //std::vector<Region> getRegions(int type);

  int getValue(int x, int y, int z) const;

public:
  float density = 0.2f;

  //CaveGenerator();

  void generate(glm::vec2 position, Block*** blocks);
};

