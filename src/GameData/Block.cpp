#include "../../include/GameData/Block.hpp"

#include "../../include/GameData/BlockStates/BlockState.hpp"
#include "../../include/GameData/GameData.hpp"

bool Block::isSolid(BlockId id) {
  return GameData::getBlockTemplate(id).isSolid;
}

bool Block::isSolid() const { return isSolid(type); }

bool Block::isTransparent(BlockId id) {
  return GameData::getBlockTemplate(id).isTransparent;
}

bool Block::isTransparent() const { return isTransparent(type); }

Block::Block() : Block{glm::ivec3{}, BlockId::BLOCK_AIR} {}

Block::Block(const glm::ivec3 &position, BlockId type)
    : Block{position, type, nullptr} {}

Block::Block(const glm::ivec3 &position, BlockId type, BlockState *state)
    : type{type}, state{state}, position{position} {}
