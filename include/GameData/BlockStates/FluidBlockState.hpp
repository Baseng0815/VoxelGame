#pragma once
#include "BlockState.hpp"

struct FluidBlockState : public BlockState {
  // default configuration for a water source block

  /* distance in blocks from source block. (max 7 blocks)
  level 8 means falling */
  int level = 0;
  float flowTime = 0.0f;
  int cornerHeights[4] = {0};

  FluidBlockState() : BlockState{} {}

  FluidBlockState(int level) : BlockState{}, level{level} {
    if (level == 0) {
      flowTime = 1.0f;
    }
  }

  FluidBlockState(int level, float flowTime)
      : BlockState{}, level{level}, flowTime{flowTime} {}
};
