#pragma once

#include "pch.h"

enum BLOCK_TYPE {
	BLOCK_AIR, BLOCK_DIRT
};

struct Block {
public:
	BLOCK_TYPE type = BLOCK_AIR;
};
