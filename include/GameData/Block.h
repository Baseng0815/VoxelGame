#pragma once

#include "BlockIds.h"

struct Block {
    BlockId type;

    bool isSolid();
};
