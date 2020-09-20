#pragma once

#include "../GameData/BiomeIds.h"
#include "../GameData/BlockIds.h"

struct Biome {
    BiomeId id;
    BlockId surfaceBlocks;
    BlockId subsurfaceBlocks;

    int surfaceHeight;
    int subsorfaceHeight;
};
