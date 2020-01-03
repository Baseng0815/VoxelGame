#pragma once

#include "BlockData.h"

struct Block {
public:
    Block(BlockType type = BlockType::BLOCK_AIR);

    BlockType type;
};
