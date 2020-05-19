#include "../../include/WorldGeneration/TerrainDesert.h"
#include "../../include/Configuration.h"
#include "../../include/Block.h"

TerrainDesert::TerrainDesert() : Terrain(55, 60) {
	baseTerrainDesert.SetPersistence(1.0f / 128.0f);
	baseTerrainDesert.SetFrequency(0.25f);
	baseTerrainDesert.SetOctaveCount(4);
	baseTerrainDesert.SetNoiseQuality(noise::NoiseQuality::QUALITY_FAST);

	terrainDesert.SetBias(-0.75f);
	terrainDesert.SetScale(0.5f);
	terrainDesert.SetSourceModule(0, baseTerrainDesert);
}

void TerrainDesert::getBlocks(glm::vec2 chunk, int cx, int cz, Block*** blocks) {
	float x = chunk.x + (float)cx / Configuration::CHUNK_SIZE;
	float z = chunk.y + (float)cz / Configuration::CHUNK_SIZE;

	int height = minHeight + (maxHeight - minHeight) * terrainDesert.GetValue(x, 0, z);
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