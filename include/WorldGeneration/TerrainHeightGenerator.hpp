#pragma once
#include <noise/noise.h>
#include <glm/glm.hpp>
#include "Biome.hpp"

using namespace noise;
using namespace noise::module;

class TerrainHeightGenerator {
private: 
    // biomes
    Perlin m_baseNoise;
    Perlin m_baseLandNoise;

    // terrains
    Perlin m_baseTerrain;
    ScaleBias m_flatTerrain;
    ScaleBias m_desertTerrain;
    ScaleBias m_beachTerrain;

    RidgedMulti m_oceanMoutainsBase;
    ScaleBias m_oceanMountains;
    Multiply m_oceanMoutainsTerrain;
    ScaleBias m_oceanTerrain;
    
    // biome selectors
    Select m_oceanSelect;
    Select m_desertSelect;
    Select m_terrainSelect;

public:
    TerrainHeightGenerator();

    void generateChunkHeight(glm::vec2 chunkPos, int** heightMap, BiomeId** biomes) const;

    int getHeight(glm::vec2 chunk, int x, int z) const;
    BiomeId getBiome(const glm::ivec2& chunk, int cx, int cz) const;    
};
