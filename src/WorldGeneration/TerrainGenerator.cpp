#include "../../include/WorldGeneration/TerrainGenerator.hpp"
#include "../../include/Configuration.hpp"
#include "../../include/Utility.hpp"

#include "../../include/GameData/BlockStates/WaterBlockState.hpp"

TerrainGenerator::TerrainGenerator() {
    // TODO: Move this to a file
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
    for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++) {
        for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
            int height = 5;
            BiomeId biomeId = data->biomes[cx][cz];
            Biome biome = getBiome(biomeId);

            for (int cy = 0; cy < height; cy++) {
                int sectionIndex = cy / Configuration::CHUNK_SECTION_SIZE;
                int scy = cy % Configuration::CHUNK_SECTION_SIZE;

                BlockId block = BlockId::BLOCK_AIR;
                if (cy < randNext(2, 4)) {
                    block = BlockId::BLOCK_BEDROCK;
                }
                else if (cy <= height - (biome.subsurfaceHeight + biome.surfaceHeight)) {
                    block = BlockId::BLOCK_STONE;
                }
                else if(cy <= height - biome.surfaceHeight) {
                    block = biome.subsurfaceBlocks;
                }
                else if(cy <= height) {
                    block = biome.surfaceBlocks;
                }
                else if(cy <= 63 && (biomeId == BiomeId::BIOME_BEACH || biomeId == BiomeId::BIOME_OCEAN)) {
                    block = BlockId::BLOCK_WATER;
                    data->stateData.createBlockState<WaterBlockState>(glm::ivec3{cx, cy, cz});
                }

                data->sections[sectionIndex].setBlock(glm::ivec3{cx, scy, cz}, block);
            }
        }
    }

    data->sections[0].blocks->print(0);
}
