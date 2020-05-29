#include "../../include/WorldGeneration/TerrainDesert.h"
#include "../../include/Configuration.h"
#include "../../include/Block.h"

TerrainDesert::TerrainDesert() : Terrain(70, 80) {	
	terrainDesert.SetSourceModule(0, baseTerrain);
	terrainDesert.SetScale(1);
}

void TerrainDesert::getBlocks(glm::vec2 chunk, int cx, int cz, Block*** blocks, int terrainHeight) {
	float x = chunk.x + (float)cx / CHUNK_SIZE;
	float z = chunk.y + (float)cz / CHUNK_SIZE;

	int height = terrainHeight;
	if(height == -1)
		height = minHeight + (maxHeight - minHeight) * terrainDesert.GetValue(x, 0, z);

	for (int cy = 0; cy < height; cy++) {
		Block block;
		if (cy < 2)
			block = Block(BlockType::BLOCK_BRICKS);
		else if (cy < height - 5)
			block = Block(BlockType::BLOCK_STONE);
		else
			block = Block(BlockType::BLOCK_SAND);		

		blocks[cx][cy][cz] = block;
	}

}

int TerrainDesert::getHeight(glm::vec2 chunk, int cx, int cz) const {
	float x = chunk.x + (float)cx / CHUNK_SIZE;
	float z = chunk.y + (float)cz / CHUNK_SIZE;

	return minHeight + (maxHeight - minHeight) * terrainDesert.GetValue(x, 0, z);
}