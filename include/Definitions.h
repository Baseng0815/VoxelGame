#pragma once

#include "BlockData.h"
#include "BiomeInfo.h"

#include <array>

#define _DEBUG

// TODO move static definitions (WINDOW_WIDTH etc.) away from runtime-loaded
// information (BLOCK_DATA etc.) and clean up include hierarchy
class Definitions {
public:
    static std::array<BlockData, (int)BlockType::NUM_BLOCKS> BLOCK_DATA;
    static std::array<BiomeInfo, (int)BiomeID::NUM_BIOMES> BIOME_INFO;

    // initial size
    static constexpr int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
    // the number of chunks in px and pz direction that are active at once
    static constexpr int CHUNK_PRELOAD_SIZE = 5;

    // how many chunks can be loaded simultaneously
    static constexpr int CHUNK_MAX_LOADING = 8;

    static constexpr int MAP_SEED_SIZE = 256;

    static constexpr int CHUNK_SIZE = 16;
    static constexpr int CHUNK_HEIGHT = 256;

    static constexpr float G_CONSTANT = 9.8f;
    static constexpr float MAX_FALL_SPEED = 55.f;

    // units per second
    static constexpr float CAM_SPEED = 16.f;

    static void loadData();
};
