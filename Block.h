#pragma once

#include "pch.h"

#include "BlockData.h"

struct Block {
public:
	Block(BlockType type = BLOCK_AIR);

	BlockType type;
};
