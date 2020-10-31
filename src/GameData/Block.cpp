#include "../../include/GameData/Block.hpp"

#include "../../include/GameData/GameData.hpp"

bool Block::isSolid() const {
    return GameData::getBlockTemplate(type).isSolid;
}

bool Block::isTransparent() const {
    return GameData::getBlockTemplate(type).isTransparent;
}

Block::Block()
    : Block{BlockId::BLOCK_AIR, nullptr} {
}

Block::Block(BlockId type, BlockState* state)
    : type{type}, state{state} {
}
