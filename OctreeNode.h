#pragma once

#include "Block.h"

struct OctreeNode {
	Block block;
	int depth;
	bool isHomogeneous;
	int x, y, z;

	// address order
	// X- Y- Z-
	// X- Y- Z+
	// X- Y+ Z-
	// X- Y+ Z+
	// X+ Y- Z-
	// X+ Y- Z+
	// X+ Y+ Z-
	// X+ Y+ Z+
	OctreeNode* nodes[8] = { nullptr };

	OctreeNode(int depth = 0, int childIndex = 0, int px = 0, int py = 0, int pz = 0, bool isHomogeneous = true, Block block = Block(BLOCK_AIR));
	int getChildIndex(int x, int y, int z);
	void free();
};
