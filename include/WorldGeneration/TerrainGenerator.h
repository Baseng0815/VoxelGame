#pragma once
#include "Biome.h"
#include <vector>

struct Block;

class TerrainGenerator {
private:
    std::vector<Biome> m_biomes;

    Biome getBiome(BiomeID id) const;
public:
    TerrainGenerator();

    void createBlocks(Block*** blocks, int** heightMap, BiomeID** biomes) const;
};