#pragma once

#include "BlockIds.hpp"
#include "BlockStates/BlockState.hpp"

#include <glm/glm.hpp>

struct Block {
  BlockId type;
  BlockState *state;
  glm::ivec3 position;

  static bool isSolid(BlockId id);
  bool isSolid() const;

  static bool isTransparent(BlockId id);
  bool isTransparent() const;

  Block();
  Block(const glm::ivec3 &position, BlockId type);
  Block(const glm::ivec3 &position, BlockId type, BlockState *state);

};
