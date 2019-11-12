#pragma once

#include "BlockData.h"

class Definitions {
public:
	static std::array<BlockData, (int)BlockType::NUM_BLOCKS> BLOCK_DATA;	

	// initial size
	static unsigned int WINDOW_WIDTH, WINDOW_HEIGHT;
	static constexpr unsigned int MAP_SEED_SIZE = 256;

	static void loadData();
};
