#pragma once

#include "../GameData/BiomeIds.hpp"
#include "../GameData/BlockIds.hpp"

struct Biome {
    BiomeId id;
    BlockId surfaceBlocks;
    BlockId subsurfaceBlocks;

    int surfaceHeight;
    int subsorfaceHeight;
};
