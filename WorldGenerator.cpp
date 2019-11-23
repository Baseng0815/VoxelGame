#include "pch.h"

// in der .cpp kann man einbinden, was man will, da entsteht kein Problem
#include "World.h"
#include "WorldGenerator.h"
#include "Definitions.h"
#include "noiseutils.h"

using namespace noise::utils;
using namespace noise::model;
using namespace noise::module;

void WorldGenerator::init(WorldType worldType) {
	m_type = worldType;

	if (m_type == WorldType::WORLD_NORMAL) {
		perlinNoise.SetSeed(rand());
		perlinNoise.SetFrequency(1.0f / 1024.0f);
		perlinNoise.SetPersistence(1.0f / 512.0f);

		// init flat terrain generation
		baseFlatTerrain.SetSeed(rand());
		baseFlatTerrain.SetPersistence(1.0f / 128.0f);
		baseFlatTerrain.SetFrequency(0.125f);
		baseFlatTerrain.SetOctaveCount(5);
		baseFlatTerrain.SetNoiseQuality(noise::NoiseQuality::QUALITY_FAST);

		flatTerrain.SetBias(-0.75f);
		flatTerrain.SetScale(0.25f);
		flatTerrain.SetSourceModule(0, baseFlatTerrain);

		terrainSelector.SetSourceModule(0, flatTerrain);
		terrainSelector.SetSourceModule(1, baseFlatTerrain);
		terrainSelector.SetControlModule(perlinNoise);

		caveSize = 5;
		caveLenght = 256;
		cavePerlin.SetSeed(rand());
		cavePerlin.SetPersistence(0.25f);
	}
}

void WorldGenerator::generateTerrain(glm::vec2 position, Chunk* chunk) const {
	if (m_type == WorldType::WORLD_NORMAL) {
		utils::NoiseMap heightMap;
		utils::NoiseMapBuilderPlane planeBuilder = NoiseMapBuilderPlane();

		planeBuilder.SetSourceModule(terrainSelector);
		planeBuilder.SetDestNoiseMap(heightMap);
		planeBuilder.SetDestSize(Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE);
		planeBuilder.SetBounds(position.x, position.x + 1, position.y, position.y + 1);

		planeBuilder.Build();

		for (int cx = 0; cx < Chunk::CHUNK_SIZE; cx++)
			for (int cz = 0; cz < Chunk::CHUNK_SIZE; cz++) {
				int height = heightMap.GetValue(cx, cz) * (float)6 + 40;
				for (int cy = 0; cy < height; cy++) {
					Block block;
					if (cy < height - 5)
						block = Block(BlockType::BLOCK_STONE);
					else if (cy < height - 1)
						block = Block(BlockType::BLOCK_DIRT);
					else
						block = Block(BlockType::BLOCK_GRASS);

					chunk->setBlock(cx, cy, cz, block);
				}
			}
	}

	chunk->updateVertices();
}

void WorldGenerator::generateUnderground(glm::vec2 position, Chunk* chunk) const {
	if (m_type == WorldType::WORLD_FLAT)
		return;

	// generate ores
	for (auto blockInfo = Definitions::BLOCK_DATA.begin(); blockInfo != Definitions::BLOCK_DATA.end(); blockInfo++) {
		for (int i = 0; i < (*blockInfo).oreData.generationCounts; i++) {
			int x = rand() % 16;
			int y = rand() % ((*blockInfo).oreData.maxHeight - (*blockInfo).oreData.minHeight) + (*blockInfo).oreData.minHeight;
			int z = rand() % 16;

			int size = (*blockInfo).oreData.size;
			generateOre(x, y, z, size, (*blockInfo).type, chunk);
		}
	}
}

void WorldGenerator::generateOre(int x, int y, int z, int size, BlockType block, Chunk* chunk) const {

}

void WorldGenerator::generateChunk(glm::vec2 position, Chunk* chunk) const {
	generateTerrain(position, chunk);
	generateUnderground(position, chunk);
}