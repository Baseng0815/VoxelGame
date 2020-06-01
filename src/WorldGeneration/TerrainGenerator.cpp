#include "../../include/WorldGeneration/TerrainGenerator.h"
#include "../../include/WorldGeneration/Biome.h"
#include "../../include/Block.h"
#include "../../include/Configuration.h"
#include "../../include/Utility.h"

TerrainGenerator::TerrainGenerator() {
	m_biomes.push_back(Biome{ BIOME_FLAT, BlockType::BLOCK_GRASS, BlockType::BLOCK_DIRT, 1, 3 });
	m_biomes.push_back(Biome{ BIOME_FLAT_TERRAIN, BlockType::BLOCK_GRASS, BlockType::BLOCK_DIRT, 1, 4 });
	m_biomes.push_back(Biome{ BIOME_DESERT, BlockType::BLOCK_SAND, BlockType::BLOCK_SAND, 1, 4 });
	m_biomes.push_back(Biome{ BIOME_BEACH, BlockType::BLOCK_SAND, BlockType::BLOCK_SAND, 1, 4 });
	m_biomes.push_back(Biome{ BIOME_OCEAN, BlockType::BLOCK_SAND, BlockType::BLOCK_SAND, 1, 3 });
}

Biome TerrainGenerator::getBiome(BiomeID id) const {
	Biome biome = {};

	for (int i = 0; i < m_biomes.size(); i++) 
		if (m_biomes[i].id == id)
			biome = m_biomes[i];
	

	return biome;
}

void TerrainGenerator::createBlocks(Block*** blocks, int** heightMap, BiomeID** biomes) const {
	for (int cx = 0; cx < CHUNK_SIZE; cx++)
		for (int cz = 0; cz < CHUNK_SIZE; cz++) {
			int height = heightMap[cx][cz];
			BiomeID biomeId = biomes[cx][cz];
			Biome biome = getBiome(biomeId);

			for (int cy = 0; cy < height; cy++) {
				Block block;
				if(cy < randNext(2, 4))
					block = Block(BlockType::BLOCK_BRICKS);
				else if(cy < height - (biome.subsorfaceHeight + biome.surfaceHeight))
					block = Block(BlockType::BLOCK_STONE);
				else if(cy < height - biome.surfaceHeight)
					block = Block(biome.subsurfaceBlocks);
				else 
					block = Block(biome.surfaceBlocks);

				blocks[cx][cy][cz] = block;
			}
		}
}