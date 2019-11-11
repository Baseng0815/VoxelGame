#pragma once

#include "BlockData.h"

class Definitions {
public:
	static std::array<BlockData, (int)BlockType::NUM_BLOCKS> BLOCK_DATA;	

	// initial size
	static unsigned int WINDOW_WIDTH, WINDOW_HEIGHT;

	static void loadData();
};
