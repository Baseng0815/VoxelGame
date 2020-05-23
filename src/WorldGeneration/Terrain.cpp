#include "../../include/WorldGeneration/Terrain.h"
#include "../../include/WorldGeneration/TerrainFlat.h"
#include "../../include/Block.h"

Terrain::Terrain(int minHeight, int maxHeight)
	: minHeight(minHeight), maxHeight(maxHeight) { }

void Terrain::getBlocks(glm::vec2 chunk, int cx, int cz, Block*** blocks, int height) {
	blocks[cx][0][cz] = Block(BlockType::BLOCK_BRICKS);

	for (int cy = 1; cy < 4; cy++) {
		blocks[cx][cy][cz] = Block(BlockType::BLOCK_DIRT);
	}

	blocks[cx][4][cz] = Block(BlockType::BLOCK_GRASS);
}

int Terrain::getHeight(glm::vec2 chunk, int cx, int cz) const {
	return 4;
}