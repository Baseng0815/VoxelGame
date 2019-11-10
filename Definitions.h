#pragma once

#include "BlockData.h"

class Definitions {
public:
	static std::array<BlockData, NUM_BLOCKS> BLOCK_DATA;

	static void loadData();
};
