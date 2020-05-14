#include "../../include/WorldGeneration/FlatTerrainGenerator.h"
#include "../../include/WorldGeneration/WorldGenerator.h"
#include "../../include/WorldGeneration/noiseutils.h"
#include "../../include/Block.h"
#include "../../include/Configuration.h"


using namespace noise::utils;

void FlatTerrainGenerator::_init() {
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

void FlatTerrainGenerator::generateTerrain(glm::vec2 position, Block*** blocks) {
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane planeBuilder = NoiseMapBuilderPlane();

	planeBuilder.SetSourceModule(terrainSelector);
	planeBuilder.SetDestNoiseMap(heightMap);
	planeBuilder.SetDestSize(Configuration::CHUNK_SIZE, Configuration::CHUNK_SIZE);
	planeBuilder.SetBounds(position.x, position.x + 1, position.y, position.y + 1);

	planeBuilder.Build();

	for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++)
		for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
			int height = heightMap.GetValue(cx, cz) * (float)12 + 60;
			for (int cy = 0; cy < height; cy++) {
				Block block;
				if (cy < 2)
					block = Block(BlockType::BLOCK_BRICKS);
				else if (cy < height - 5)
					block = Block(BlockType::BLOCK_STONE);
				else if (cy < height - 1)
					block = Block(BlockType::BLOCK_DIRT);
				else {
					if (height > 56)
						block = Block(BlockType::BLOCK_GRASS);
					else
						block = Block(BlockType::BLOCK_SAND);
				}

				blocks[cx][cy][cz] = block;
			}
		}
}