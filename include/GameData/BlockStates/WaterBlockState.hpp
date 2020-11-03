#pragma once
#include "BlockState.hpp"

struct WaterBlockState : public BlockState {
    // default configuration for a water source block
    int level = 0;
    bool falling = false;

    WaterBlockState()
        : BlockState{} {
    }

    WaterBlockState(bool falling)
        : BlockState{},
          falling{true} {
    }

    WaterBlockState(int level)
        : BlockState{},
          level{level} {
    }

    WaterBlockState(bool falling, int level)
        : BlockState{},
          falling{falling},
          level{level} {
    }

    bool operator==(const WaterBlockState& state) {
        return level == state.level && falling == state.falling;
    }
};