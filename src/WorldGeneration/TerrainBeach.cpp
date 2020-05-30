#include "../../include/WorldGeneration/TerrainBeach.h"

#include "../../include/Configuration.h"
#include "../../include/Utility.h"
#include "../../include/Block.h"

TerrainBeach::TerrainBeach() : Terrain(63, 70) {

}

void TerrainBeach::getBlocks(int cx, int cz, Block*** blocks, int terrainHeight) {		
	for (int cy = 0; cy < terrainHeight; cy++) {
		Block block;
		if (cy < randNext(2, 4))
			block = Block(BlockType::BLOCK_BRICKS);
		else if (cy < terrainHeight - 4)
			block = Block(BlockType::BLOCK_STONE);
		else
			block = Block(BlockType::BLOCK_SAND);

		blocks[cx][cy][cz] = block;
	}
}

int TerrainBeach::getHeight(glm::ivec2 chunk, int cx, int cz) const {
	float x = chunk.x + (float)cx / CHUNK_SIZE;
	float z = chunk.y + (float)cz / CHUNK_SIZE;

	return minHeight + (maxHeight - minHeight) * baseTerrain.GetValue(x, 0, z);
}