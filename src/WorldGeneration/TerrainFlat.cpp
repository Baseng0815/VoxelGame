#include "../../include/WorldGeneration/TerrainFlat.h"
#include "../../include/Configuration.h"
#include "../../include/Block.h"

TerrainFlat::TerrainFlat() : Terrain(65, 75) {

	flatTerrain.SetSourceModule(0, baseTerrain);
	flatTerrain.SetScale(1);
}

void TerrainFlat::getBlocks(int cx, int cz, Block*** blocks, int terrainHeight) {	
	for (int cy = 0; cy < terrainHeight; cy++) {
		Block block;
		if (cy < 2)
			block = Block(BlockType::BLOCK_BRICKS);
		else if (cy < terrainHeight - 5)
			block = Block(BlockType::BLOCK_STONE);
		else if (cy < terrainHeight - 1)
			block = Block(BlockType::BLOCK_DIRT);
		else
			block = Block(BlockType::BLOCK_GRASS);

		blocks[cx][cy][cz] = block;
	}

}

int TerrainFlat::getHeight(glm::ivec2 chunk, int cx, int cz) const {
	float x = chunk.x + (float)cx / CHUNK_SIZE;
	float z = chunk.y + (float)cz / CHUNK_SIZE;

	return minHeight + (maxHeight - minHeight) * flatTerrain.GetValue(x, 0, z);
}