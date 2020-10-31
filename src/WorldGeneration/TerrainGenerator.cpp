#include "../../include/WorldGeneration/TerrainGenerator.hpp"
#include "../../include/Configuration.hpp"
#include "../../include/Utility.hpp"

#include "../../include/GameData/BlockStates/WaterBlockState.hpp"

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

void TerrainGenerator::createBlocks(GenerationData* data, int** heightMap) const {
    data->stateData.push_back(new BlockState{BlockState::empty()});
    data->stateData.push_back(new WaterBlockState{});

    data->blockData.emplace_back(Block{BlockId::BLOCK_AIR});
    data->blockData.emplace_back(Block{BlockId::BLOCK_BRICKS});
    data->blockData.emplace_back(Block{BlockId::BLOCK_STONE});
    data->blockData.emplace_back(Block{BlockId::BLOCK_WATER});

    for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++)
        for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
            int height = heightMap[cx][cz];
            BiomeId biomeId = data->biomes[cx][cz];
            Biome biome = getBiome(biomeId);
            int subsurface = -1;
            int surface = -1;

            int index = 0;
            while (subsurface == -1 && surface == -1 && index < data->blockData.size()) {
                if (data->blockData[index].type == biome.subsurfaceBlocks) {
                    subsurface = index;
                }

                if (data->blockData[index].type == biome.surfaceBlocks) {
                    surface = index;
                }

                index++;
            }

            if (subsurface == -1) {
                data->blockData.emplace_back(Block{biome.subsurfaceBlocks});
                subsurface = index++;

                if (biome.surfaceBlocks == biome.subsurfaceBlocks) {
                    surface = subsurface;
                }                
            }

            if (surface == -1) {
                data->blockData.emplace_back(Block{biome.surfaceBlocks});
                surface = index++;
            }

            for (int cy = 0; cy < Configuration::CHUNK_HEIGHT; cy++) {
                int block;
                if (cy < randNext(2, 4))
                    // bricks
                    block = 1;
                else if (cy <= height - (biome.subsorfaceHeight + biome.surfaceHeight))
                    // stone
                    block = 2;
                else if (cy <= height - biome.surfaceHeight)
                    block = subsurface;
                else if (cy <= height) {
                    block = surface;
                }
                else if (cy <= 63) {
                    // water default state index and block index
                    block = 1 << 16 | 3;
                }
                else {
                    // air
                    block = 0;
                }

                data->blocks[cx][cy][cz] = block;
            }
        }
}
