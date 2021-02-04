#include "../../include/WorldGeneration/TerrainGenerator.hpp"
#include "../../include/Configuration.hpp"
#include "../../include/Utility/Utility.hpp"

#include "../../include/GameData/BlockStates/WaterBlockState.hpp"

TerrainGenerator::TerrainGenerator() {
    m_biomes.emplace_back(BiomeId::BIOME_FLAT, BlockId::BLOCK_GRASS, BlockId::BLOCK_DIRT, 1, 3);
    m_biomes.emplace_back(BiomeId::BIOME_FLAT_TERRAIN, BlockId::BLOCK_GRASS, BlockId::BLOCK_DIRT, 1, 4);
    m_biomes.emplace_back(BiomeId::BIOME_DESERT, BlockId::BLOCK_SAND, BlockId::BLOCK_SAND, 1, 4);
    m_biomes.emplace_back(BiomeId::BIOME_BEACH, BlockId::BLOCK_SAND, BlockId::BLOCK_SAND, 1, 4);
    m_biomes.emplace_back(BiomeId::BIOME_OCEAN, BlockId::BLOCK_SAND, BlockId::BLOCK_SAND, 1, 3);
}

Biome TerrainGenerator::getBiome(BiomeId id) const {
    Biome biome = {};

    for (int i = 0; i < m_biomes.size(); i++)
        if (m_biomes[i].id == id)
            biome = m_biomes[i];

    return biome;
}

void TerrainGenerator::createBlocks(GenerationData* data, int** heightMap) const {
    for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++)
        for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
            int height = heightMap[cx][cz];
            BiomeId biomeId = data->biomes[cx][cz];
            Biome biome = getBiome(biomeId);

            for (int cy = 0; cy < Configuration::CHUNK_HEIGHT; cy++) {
                BlockId block = BlockId::BLOCK_AIR;
                if (cy < randNext(2, 4))
                    // bricks
                    block = BlockId::BLOCK_BEDROCK;
                else if (cy <= height - (biome.subsurfaceHeight + biome.surfaceHeight))
                    // stone
                    block = BlockId::BLOCK_STONE;
                else if (cy <= height - biome.surfaceHeight)
                    block = biome.subsurfaceBlocks;
                else if (cy <= height) {
                    block = biome.surfaceBlocks;
                }
                else if (cy <= 63) {
                    // water default state index and block index
                    block = BlockId::BLOCK_WATER;
                    data->stateData.createBlockState<WaterBlockState>(glm::vec3{cx, cy, cz});
                }

                data->blocks[cx][cy][cz] = block;
            }
        }
}
