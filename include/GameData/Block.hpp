#pragma once

#include "BlockIds.hpp"
#include "BlockStates/BlockState.hpp"

#include <glm/glm.hpp>

struct Block {
  BlockId type;
  BlockState *state;
  glm::ivec3 position;

  bool isSolid() const;
  bool isTransparent() const;

  Block();
  Block(const glm::ivec3 &position, BlockId type);
  Block(const glm::ivec3 &position, BlockId type, BlockState *state);
};
