#pragma once
#include "Biome.hpp"
#include "GenerationData.hpp"

#include <vector>

class TerrainGenerator {
private:
    std::vector<Biome> m_biomes;

    Biome getBiome(BiomeId id) const;
public:
    TerrainGenerator();

    void createBlocks(GenerationData* data, int** heightMap) const;
};
