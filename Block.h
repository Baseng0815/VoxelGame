#pragma once

#include "pch.h"

enum BLOCK_TYPE {
	BLOCK_AIR, BLOCK_DIRT, BLOCK_COBBLESTONE, BLOCK_GRASS, BLOCK_STONE, NUM_BLOCKS
};

struct Block {
public:
	Block(BLOCK_TYPE type = BLOCK_AIR);

	BLOCK_TYPE type;
};
