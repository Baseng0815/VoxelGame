#pragma once

#include <glm/glm.hpp>

#include "../Configuration.h"

#include <vector>

typedef std::array<glm::vec2, 4> FaceUVs;
typedef std::array<FaceUVs, 6> BlockUVs;
typedef std::array<BlockUVs, (int)BlockType::NUM_BLOCKS> BlockUVsArray;

struct AtlasComponent {
    int numRows, numCols;
    float uvXpT, uvYpT;

    BlockUVsArray blockUVsArray;

    AtlasComponent(int width, int height, int tileSize);
};
