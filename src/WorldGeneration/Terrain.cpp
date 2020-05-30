#include "../../include/WorldGeneration/Terrain.h"
#include "../../include/WorldGeneration/TerrainFlat.h"
#include "../../include/Block.h"
#include "../../include/Configuration.h"

Terrain::Terrain(int minHeight, int maxHeight)
	: minHeight(minHeight), maxHeight(maxHeight) {

	basePerlin.SetSeed(0x849DAF8975FAB);
	basePerlin.SetLacunarity(1.5f);
	basePerlin.SetFrequency(0.1);
	basePerlin.SetOctaveCount(6);
	basePerlin.SetNoiseQuality(noise::NoiseQuality::QUALITY_FAST);

	// set output to values between 0 and 1
	baseTerrain.SetSourceModule(0, basePerlin);
	baseTerrain.SetScale(0.5);
	baseTerrain.SetBias(0.5);
}

void Terrain::getBlocks(int cx, int cz, Block*** blocks, int height) {
	blocks[cx][0][cz] = Block(BlockType::BLOCK_BRICKS);

	for (int cy = 1; cy < 4; cy++) {
		blocks[cx][cy][cz] = Block(BlockType::BLOCK_DIRT);
	}

	blocks[cx][4][cz] = Block(BlockType::BLOCK_GRASS);
}

int Terrain::getHeight(glm::ivec2 chunk, int cx, int cz) const {
	return 4;
}

int** Terrain::generateHeightMap(glm::ivec2 chunk) const {
	int** map = new int* [CHUNK_SIZE];
	for (int cx = 0; cx < CHUNK_SIZE; cx++) {
		map[cx] = new int[CHUNK_SIZE];
		for (int cz = 0; cz < CHUNK_SIZE; cz++)
			map[cx][cz] = getHeight(chunk, cx, cz);
	}

	return map;
}