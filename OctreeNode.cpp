#include "pch.h"

#include "Chunk.h"
#include "OctreeNode.h"

OctreeNode::OctreeNode(int depth, int childIndex, int px, int py, int pz, bool isHomogeneous, Block block)
	: depth(depth), isHomogeneous(isHomogeneous), block(block) {

	int offset = Chunk::CHUNK_SIZE * (1 - 1 / std::pow(2, depth));

	x = px;
	y = py;
	z = pz;

	if (childIndex >= 4) {
		x += offset;
		childIndex -= 4;
	}
	if (childIndex >= 2) {
		y += offset;
		childIndex -= 2;
	}
	if (childIndex >= 1)
		z += offset;
}

int OctreeNode::getChildIndex(int x, int y, int z) {
	int childIndex = 0;
	childIndex += (x > this->x) ? 4 : 0;
	childIndex += (y > this->y) ? 2 : 0;
	childIndex += (z > this->z) ? 1 : 0;
}

void OctreeNode::free() {
	if (!isHomogeneous) {
		for (int i = 0; i < 8; i++) {
			nodes[i]->free();
			delete nodes[i];
		}
	}
}
