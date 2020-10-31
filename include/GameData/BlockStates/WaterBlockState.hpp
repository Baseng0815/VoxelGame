#pragma once
#include "BlockState.hpp"

struct WaterBlockState : public BlockState {
    // default configuration for a water source block
    int level = 0;
    bool falling = false;

    WaterBlockState()
        : BlockState{BlockId::BLOCK_WATER} {
    }

    bool operator==(const WaterBlockState& state) {
        return level == state.level && falling == state.falling;
    }
};