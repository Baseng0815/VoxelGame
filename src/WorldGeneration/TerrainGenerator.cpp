#include "../../include/WorldGeneration/TerrainGenerator.hpp"
#include "../../include/Configuration.hpp"
#include "../../include/Utility.hpp"

TerrainGenerator::TerrainGenerator() {
    m_biomes.push_back({BiomeId::BIOME_FLAT, BlockId::BLOCK_GRASS, BlockId::BLOCK_DIRT, 1, 3});
    m_biomes.push_back({BiomeId::BIOME_FLAT_TERRAIN, BlockId::BLOCK_GRASS, BlockId::BLOCK_DIRT, 1, 4});
    m_biomes.push_back({BiomeId::BIOME_DESERT, BlockId::BLOCK_SAND, BlockId::BLOCK_SAND, 1, 4});
    m_biomes.push_back({BiomeId::BIOME_BEACH, BlockId::BLOCK_SAND, BlockId::BLOCK_SAND, 1, 4});
    m_biomes.push_back({BiomeId::BIOME_OCEAN, BlockId::BLOCK_SAND, BlockId::BLOCK_SAND, 1, 3});
}

Biome TerrainGenerator::getBiome(BiomeId id) const {
    Biome biome = {};

    for (int i = 0; i < m_biomes.size(); i++) 
        if (m_biomes[i].id == id)
            biome = m_biomes[i];


    return biome;
}

void TerrainGenerator::createBlocks(Block*** blocks, int** heightMap, BiomeId** biomes) const {
    for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++)
        for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
            int height = heightMap[cx][cz];
            BiomeId biomeId = biomes[cx][cz];
            Biome biome = getBiome(biomeId);

            for (int cy = 0; cy < height; cy++) {
                Block block;
                if (cy < randNext(2, 4))
                    block = Block {BlockId::BLOCK_BRICKS};
                else if (cy < height - (biome.subsorfaceHeight + biome.surfaceHeight))
                    block = Block {BlockId::BLOCK_STONE};
                else if (cy < height - biome.surfaceHeight)
                    block = Block {biome.subsurfaceBlocks};
                else
                    block = Block {biome.surfaceBlocks};

                blocks[cx][cy][cz] = block;
            }
        }
}
