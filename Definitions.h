#pragma once

#include "BlockData.h"

class Definitions {
public:
	static std::array<BlockData, (int)BlockType::NUM_BLOCKS> BLOCK_DATA;	

	static void loadData();
};
