#pragma once

#include "../GameData/BiomeIds.hpp"
#include "../GameData/BlockIds.hpp"

struct Biome {
    BiomeId id;
    BlockId surfaceBlocks;
    BlockId subsurfaceBlocks;

    int surfaceHeight;
    int subsurfaceHeight;

    Biome() {}

    Biome(BiomeId id, BlockId surfaceBlocks, BlockId subsurfaceBlocks, int surfaceHeight, int subsurfaceHeight)
        : id{id}, surfaceBlocks{surfaceBlocks}, subsurfaceBlocks{subsurfaceBlocks}, surfaceHeight{surfaceHeight}, subsurfaceHeight{subsurfaceHeight} {
    }
};
