#pragma once
#include <glm/glm.hpp>
#include "../../include/WorldGeneration/Biome.h"
#include "../../include/Utility.h"
#include <map>

struct Block;

class ForrestGenerator {
private:
    float threshold = 0.025;

    std::unordered_map<glm::vec2, std::vector<std::tuple<glm::vec3, BlockType>>, Utility::HashFunctionVec2> generationData;

    void setBlock(glm::vec2 chunk, int cx, int cy, int cz, Block*** blocks, BlockType type = BlockType::BLOCK_LEAVES);

  public:
    void generateTrees(glm::vec2 chunk, Block*** blocks, BiomeID** biomes);
};