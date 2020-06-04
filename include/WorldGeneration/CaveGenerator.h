#pragma once
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <noise/noise.h>
#include "../Utility.h"
#include "PerlinWorm.h"

class Block;
using namespace noise::module;

class CaveGenerator {
private:
    Perlin cavePerlin;

    std::vector<PerlinWorm> m_worms;
    std::unordered_map<glm::vec2, std::vector<WormPart>, HashFunction> m_cache;

    void createWorm(glm::vec2 chunk);

public:
    CaveGenerator();        

    void generate(glm::vec2 position, Block*** blocks);
};

