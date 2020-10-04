#pragma once

#include "../GameData/BlockIds.hpp"

#include <array>
#include <vector>
#include <glm/glm.hpp>

using FaceUVs = std::array<glm::vec2, 4>;
using BlockUVs = std::array<FaceUVs, 6>;
using FaceUVsArray = std::vector<FaceUVs>;
using BlockUVsArray = std::array<BlockUVs, (size_t)BlockId::NUM_BLOCKS>;

class Texture;

struct TextureAtlas {
    int rows, cols;
    float uvXpT, uvYpT;

    FaceUVsArray faceUVsArray;
    BlockUVsArray blockUVsArray;

    TextureAtlas(int rows, int cols);

    const BlockUVs &operator[](const BlockId &id) const;
};
