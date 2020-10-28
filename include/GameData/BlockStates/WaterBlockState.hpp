#pragma once
#include "BlockState.hpp"

struct WaterBlockState : public BlockState<BlockId::BLOCK_WATER> {
    // default configuration for a water source block
    int level = 0;    
    bool falling = false;    
};