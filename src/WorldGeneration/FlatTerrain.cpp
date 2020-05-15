#include "../../include/WorldGeneration/FlatTerrain.h"
#include "../../include/Configuration.h"
#include "../../include/Block.h"

FlatTerrain::FlatTerrain() : Terrain(55, 65) {
	//perlinNoise.SetSeed(*seeds);
	perlinNoise.SetFrequency(1.0f / 512.0f);
	perlinNoise.SetPersistence(1.0f / 512.0f);

	// init flat terrain generation
	//baseFlatTerrain.SetSeed((*seeds + 1));
	baseFlatTerrain.SetPersistence(1.0f / 128.0f);
	baseFlatTerrain.SetFrequency(0.25f);
	baseFlatTerrain.SetOctaveCount(4);
	baseFlatTerrain.SetNoiseQuality(noise::NoiseQuality::QUALITY_FAST);

	flatTerrain.SetBias(-0.75f);
	flatTerrain.SetScale(0.5f);
	flatTerrain.SetSourceModule(0, baseFlatTerrain);

	terrainSelector.SetSourceModule(0, flatTerrain);
	terrainSelector.SetSourceModule(1, baseFlatTerrain);
	terrainSelector.SetControlModule(perlinNoise);
}

void FlatTerrain::getBlocks(glm::vec2 chunk, int cx, int cz, Block*** blocks) {
	float x = chunk.x + (float)cx / Configuration::CHUNK_SIZE;
	float z = chunk.y + (float)cz / Configuration::CHUNK_SIZE;

	int height = minHeight + (maxHeight - minHeight) * terrainSelector.GetValue(x, 0, z);
	for (int cy = 0; cy < height; cy++) {
		Block block;
		if (cy < 2)
			block = Block(BlockType::BLOCK_BRICKS);
		else if (cy < height - 5)
			block = Block(BlockType::BLOCK_STONE);
		else if (cy < height - 1)
			block = Block(BlockType::BLOCK_DIRT);
		else
			block = Block(BlockType::BLOCK_GRASS);

		blocks[cx][cy][cz] = block;
	}

}