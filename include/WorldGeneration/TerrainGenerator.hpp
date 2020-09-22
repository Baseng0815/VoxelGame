#pragma once
#include "Biome.hpp"
#include <vector>

struct Block;

class TerrainGenerator {
private:
    std::vector<Biome> m_biomes;

    Biome getBiome(BiomeId id) const;
public:
    TerrainGenerator();

    void createBlocks(Block*** blocks, int** heightMap, BiomeId** biomes) const;
};
