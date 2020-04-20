// in der .cpp kann man einbinden, was man will, da entsteht kein Problem
#include "../include/World.h"
#include "../include/WorldGenerator.h"
#include "../include/Definitions.h"
#include "../include/noiseutils.h"

using namespace noise::utils;
using namespace noise::model;
using namespace noise::module;

WorldGenerator::WorldGenerator(const WorldGenerator& generator) {
	biomes = generator.biomes;

	int seeds[] = { generator.perlinNoise.GetSeed(), generator.baseFlatTerrain.GetSeed() };
	init(generator.m_type, seeds);
}

WorldGenerator& WorldGenerator::operator=(const WorldGenerator& generator) {
	biomes = generator.biomes;

	int seeds[] = { generator.perlinNoise.GetSeed(), generator.baseFlatTerrain.GetSeed() };
	init(generator.m_type, seeds);

	m_caveGenerator = generator.m_caveGenerator;

	return *this;
}

void WorldGenerator::init(WorldType worldType, int* seeds) {
	m_type = worldType;

	if (m_type == WorldType::WORLD_NORMAL) {
		perlinNoise.SetSeed(*seeds);
		perlinNoise.SetFrequency(1.0f / 512.0f);
		perlinNoise.SetPersistence(1.0f / 512.0f);

		// init flat terrain generation
		baseFlatTerrain.SetSeed((*seeds + 1));
		baseFlatTerrain.SetPersistence(1.0f / 128.0f);
		baseFlatTerrain.SetFrequency(0.125f);
		baseFlatTerrain.SetOctaveCount(4);
		baseFlatTerrain.SetNoiseQuality(noise::NoiseQuality::QUALITY_FAST);

		flatTerrain.SetBias(-0.75f);
		flatTerrain.SetScale(0.25f);
		flatTerrain.SetSourceModule(0, baseFlatTerrain);

		terrainSelector.SetSourceModule(0, flatTerrain);
		terrainSelector.SetSourceModule(1, baseFlatTerrain);
		terrainSelector.SetControlModule(perlinNoise);

		m_caveGenerator = CaveGenerator();
	}
}

void WorldGenerator::init(WorldType worldType) {
	int seeds[] = { rand(), rand() };
	init(worldType, seeds);
}

void WorldGenerator::generateTerrain(glm::vec2 position, Block*** blocks) {
	if (m_type == WorldType::WORLD_NORMAL) {
		utils::NoiseMap heightMap;
		utils::NoiseMapBuilderPlane planeBuilder = NoiseMapBuilderPlane();

		planeBuilder.SetSourceModule(terrainSelector);
		planeBuilder.SetDestNoiseMap(heightMap);
		planeBuilder.SetDestSize(Definitions::CHUNK_SIZE, Definitions::CHUNK_SIZE);
		planeBuilder.SetBounds(position.x, position.x + 1, position.y, position.y + 1);

		planeBuilder.Build();

		for (int cx = 0; cx < Definitions::CHUNK_SIZE; cx++)
			for (int cz = 0; cz < Definitions::CHUNK_SIZE; cz++) {
				int height = heightMap.GetValue(cx, cz) * (float)6 + 60;
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
	}
}

void WorldGenerator::generateUnderground(glm::vec2 position, Block*** blocks) {
	if (m_type == WorldType::WORLD_FLAT)
		return;

	//// generate ores
	for (int b = 0; b < (int)BlockType::NUM_BLOCKS; b++) {
		BlockData blockData = Definitions::BLOCK_DATA[b];
		for (int i = 0; i < blockData.oreData.generationCounts; i++) {
			int x = rand() % 16;
			int y = rand() % (blockData.oreData.maxHeight - blockData.oreData.minHeight) + blockData.oreData.minHeight;
			int z = rand() % 16;

			int size = blockData.oreData.size;
			generateOre(x, y, z, size, (BlockType)b, blocks);
		}
	}

	// caves
	m_caveGenerator.generate(position, blocks);
}

void WorldGenerator::generateOre(int x, int y, int z, int size, BlockType block, Block*** blocks) const {
	for (int i = 0; i < size; i += 4) {
		float x1 = x + i;
		float z1 = z + i;

		for (int k = 0; k < 2; k++)
			for (int j = 0; j < 2; j++)
				if (i + k + j < size) {
					int x2 = x1 + k;
					int y2 = y + j;
					int z2 = z1;

					if (x2 >= 0 && x2 < 16 && y2 >= 0 && y2 < 256 && z2 >= 0 && z2 < 16) {
						if (blocks[x2][y2][z2].type == BlockType::BLOCK_STONE)
							blocks[x2][y2][z2] = Block(block);
					}
				}
	}
}

void WorldGenerator::generate(glm::vec2 position, Block*** blocks) {
	generateTerrain(position, blocks);
	generateUnderground(position, blocks);
}
