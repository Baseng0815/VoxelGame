#pragma once

#include "pch.h"

enum BlockType {
	BLOCK_AIR, BLOCK_DIRT, BLOCK_COBBLESTONE, BLOCK_GRASS, BLOCK_STONE, NUM_BLOCKS
};

struct Block {
public:
	Block(BlockType type = BLOCK_AIR);

	BlockType type;
};
