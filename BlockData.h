#pragma once

#include "pch.h"
#include "OreData.h"

// map names to IDs and vice versa
enum class BlockType : char {
	BLOCK_AIR, BLOCK_GRASS, BLOCK_STONE, BLOCK_WOOD, BLOCK_STONE_BRICKS, BLOCK_STONE_FRAMED,
	BLOCK_BRICKS, BLOCK_TNT, BLOCK_STONE_CRACKED, BLOCK_STONE_COBBLE, BLOCK_SAND, BLOCK_GRAVEL,
	BLOCK_IRON, BLOCK_GOLD, BLOCK_DIAMOND, BLOCK_EMERALD, BLOCK_ORE_GOLD, BLOCK_ORE_IRON, BLOCK_ORE_COAL,
	BLOCK_BOOLSHELF, BLOCK_STONE_MOSSY, BLOCK_DIRT,
	NUM_BLOCKS
};

struct BlockData {
	bool drawBlock = true;
	// texture IDs
	int tid_up = 0, tid_px = 0, tid_nx = 0, tid_pz = 0, tid_nz = 0, tid_bot = 0;
	
	OreData oreData;
};