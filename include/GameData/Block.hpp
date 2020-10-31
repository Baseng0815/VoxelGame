#pragma once

#include "BlockIds.hpp"
#include "BlockStates/BlockState.hpp"

struct Block {
    BlockId type;
    BlockState* state;

    bool isSolid() const;
    bool isTransparent() const;

    Block();
    Block(BlockId type, BlockState* state);
};
