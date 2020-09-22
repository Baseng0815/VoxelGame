#pragma once

#include "BlockIds.hpp"

struct Block {
    BlockId type;

    bool isSolid();
};
