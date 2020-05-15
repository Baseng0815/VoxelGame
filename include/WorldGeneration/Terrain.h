#pragma once
#include <glm/glm.hpp>

struct Block;

class Terrain {
protected:
	int minHeight, maxHeight;	

public:		
	Terrain(int minHeight, int maxHeight);

	virtual void getBlocks(glm::vec2 chunk, int cx, int cy, Block*** blocks);
};