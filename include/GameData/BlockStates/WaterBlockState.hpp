#pragma once
#include "BlockState.hpp"

struct WaterBlockState : public BlockState {
    // default configuration for a water source block
    int level = 0;    
    bool falling = false;    

    
};