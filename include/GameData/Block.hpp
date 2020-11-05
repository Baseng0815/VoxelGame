#pragma once

#include "BlockIds.hpp"

#include <glm/glm.hpp>

struct BlockState;

struct Block {
    BlockId type;
    BlockState* state;
    glm::vec3 position;

    bool isSolid() const;
    bool isTransparent() const;

    Block();
    Block(const glm::vec3& position, BlockId type);
    Block(const glm::vec3& position, BlockId type, BlockState* state);
};
