#include "../../include/WorldGeneration/TerrainOcean.h"
#include "../../include/Configuration.h"
#include "../../include/Block.h"

TerrainOcean::TerrainOcean() : Terrain(5, 20) {
	oceanMoutainsInverted.SetFrequency(0.01);
	oceanMoutainsInverted.SetOctaveCount(5);
	oceanMoutainsInverted.SetLacunarity(1.5);

	oceanMoutains.SetSourceModule(0, oceanMoutainsInverted);
	oceanMoutains.SetBias(1);
	oceanMoutains.SetScale(0.5);

	oceanTerrain.SetSourceModule(0, baseTerrain);
	oceanTerrain.SetSourceModule(1, oceanMoutains);
}

void TerrainOcean::getBlocks(glm::vec2 chunk, int cx, int cz, Block*** blocks, int terrainHeight) {
	float x = chunk.x + (float)cx / CHUNK_SIZE;
	float z = chunk.y + (float)cz / CHUNK_SIZE;

	int height = terrainHeight;
	if (height == -1)
		height = minHeight + ((maxHeight - minHeight) * (float)oceanTerrain.GetValue(x, 0, z));

	for (int cy = 0; cy < height; cy++) {
		Block block;
		if (cy < 2)
			block = Block(BlockType::BLOCK_BRICKS);
		else if (cy < height - 5)
			block = Block(BlockType::BLOCK_STONE);
		else {
			if (rand() % 2 == 0)
				block = Block(BlockType::BLOCK_SAND);
			else
				block = Block(BlockType::BLOCK_GRAVEL);
		}

		blocks[cx][cy][cz] = block;
	}	
}

int TerrainOcean::getHeight(glm::vec2 chunk, int cx, int cz) const {
	float x = chunk.x + (float)cx / CHUNK_SIZE;
	float z = chunk.y + (float)cz / CHUNK_SIZE;
	
	return minHeight + ((maxHeight - minHeight) * (float)oceanTerrain.GetValue(x, 0, z));
}