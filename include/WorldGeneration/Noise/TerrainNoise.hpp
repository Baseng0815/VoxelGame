#pragma once

#include <noise/noise.h>
#include <glm/glm.hpp>

#include "noiseutils.hpp"
#include "../../GameData/BiomeIds.hpp"

using namespace noise;
using namespace noise::module;


class TerrainNoise {
private:
    // biomes
    Perlin m_baseNoise;
    Perlin m_baseLandNoise;

    void initBaseModules();

    // terrains
    Perlin m_baseTerrain;
    ScaleBias m_flatTerrain;
    ScaleBias m_desertTerrain;
    ScaleBias m_beachTerrain;

    RidgedMulti m_oceanMoutainsBase;
    ScaleBias m_oceanMountains;
    Multiply m_oceanMoutainsTerrain;
    ScaleBias m_oceanTerrain;

    void initTerrainModules();
    
    // biome selectors
    Select m_oceanSelect;
    Select m_desertSelect;
    Select m_terrainSelect;

    void initSelectorModules();

public:    
    TerrainNoise();

    int getHeight(const glm::ivec2& chunk, const int cx, const int cz) const;
    BiomeId getBiome(const glm::ivec2& chunk, const int cx, const int cz) const;
};
