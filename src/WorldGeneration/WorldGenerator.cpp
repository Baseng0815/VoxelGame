// in der .cpp kann man einbinden, was man will, da entsteht kein Problem
#include "../../include/WorldGeneration/WorldGenerator.h"
#include "../../include/WorldGeneration/noiseutils.h"
#include "../../include/Block.h"
#include "../../include/BlockData.h"
#include "../../include/Configuration.h"
#include "../../include/Utility.h"

using namespace noise::utils;
using namespace noise::model;
using namespace noise::module;

void outputData(const Module& module, const char* outputName, int size) {
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;

	heightMapBuilder.SetSourceModule(module);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(size, size);
	heightMapBuilder.SetBounds(0.0, size, 0.0, size);
	heightMapBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.Render();

	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename(outputName);
	writer.WriteDestFile();
}

WorldGenerator::WorldGenerator(WorldType worldType) {
	m_type = worldType;	
	m_terrainGradient.SetSourceModule(0, m_terrainGenerator);
	m_terrainGradient.SetStepSize(20);

	outputData(m_terrainGenerator, "terrain.bmp", 512);
	outputData(m_terrainGradient, "gradient.bmp", 512);
}

WorldGenerator& WorldGenerator::operator=(const WorldGenerator& generator) {
	m_caveGenerator = generator.m_caveGenerator;

	return *this;
}

void WorldGenerator::generateOres(BiomeID** biomes, Block*** blocks) const {
	for (BlockType block = BlockType::BLOCK_ORE_GOLD; block <= BlockType::BLOCK_ORE_COAL; ) {
		BlockData blockData = Configuration::getBlockData(block);

		for (int c = 0; c < blockData.oreData.generationCounts; c++) {
			int x = rand() % CHUNK_SIZE;
			int y = rand() % (blockData.oreData.maxHeight - blockData.oreData.minHeight) + blockData.oreData.minHeight;
			int z = rand() % CHUNK_SIZE;	
			glm::ivec3 orePos = glm::ivec3(x, y, z);

			double a = pow(3 * blockData.oreData.size / (16 * PI), 1 / 3.0);

			//		x^2		y^2		z^2
			// 1 = ----- + ----- + -----
			//		a^2		a^2		4a^2

			for(int x1 = -a; x1 <= a; x1++)
				for (int z1 = -2 * a; z1 <= 2 * a; z1++) {
					double fSquared = pow(a, 2) - pow(x1, 2) - (pow(z1, 2) / pow(a, 2));

					if (fSquared >= 0) {
						double ymin = -pow(fSquared, 0.5);
						double ymax = -ymin;

						for (int y1 = ymin; y1 <= ymax; y1++) {
							glm::ivec3 pos = orePos + glm::ivec3(x1, y1, z1);							

							if (blocks[pos.x][pos.y][pos.z].type == BlockType::BLOCK_STONE) {
								blocks[pos.x][pos.y][pos.z] = Block(block);
							}
						}
					}
				}
		}
		
		block = (BlockType)((int)block + 1);
	}
}


void WorldGenerator::generate(glm::ivec2 position, BiomeID** biomes, Block*** blocks) {
	for(int cx = 0; cx < CHUNK_SIZE; cx++)
		for(int cz = 0; cz < CHUNK_SIZE; cz++) {
			float x = position.x * CHUNK_SIZE + cx;
			float z = position.y * CHUNK_SIZE + cz;

			Biome biome = m_terrainGenerator.getBiome(x, z);
			biomes[cx][cz] = biome.id;

			int height = m_terrainGenerator.GetValue(x, 0, z);
			
			biome.generator->getBlocks(cx, cz, blocks, height);
		}

	//generateOres(biomes, blocks);

	//m_caveGenerator.generate(position, blocks);
}
