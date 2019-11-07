#pragma once

#include "pch.h"

enum class BLOCK_TYPE {
	BLOCK_AIR, BLOCK_DIRT
};

struct Block {
public:
	Block(BLOCK_TYPE type = BLOCK_TYPE::BLOCK_AIR);

	BLOCK_TYPE type = BLOCK_TYPE::BLOCK_AIR;
};
