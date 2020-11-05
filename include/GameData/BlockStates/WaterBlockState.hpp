#pragma once
#include "BlockState.hpp"

struct WaterBlockState : public BlockState {
    // default configuration for a water source block

    /* distance in blocks from source block. (max 7 blocks)
    level 8 means falling*/
    int level = 0;

    WaterBlockState()
        : BlockState{} {
    }

    WaterBlockState(int level)
        : BlockState{}, level{level} {
    }
};
