#include "../../../include/WorldGeneration/Noise/TerrainNoise.hpp"
#include "../../../include/Configuration.hpp"

TerrainNoise::TerrainNoise() {
    initBaseModules();
    initTerrainModules();
    initSelectorModules();
}

void TerrainNoise::initBaseModules() {
    m_baseNoise.SetFrequency(0.005);
    m_baseNoise.SetOctaveCount(4);
    m_baseNoise.SetPersistence(0.25);
    m_baseNoise.SetSeed(154541345);
    m_baseNoise.SetNoiseQuality(NoiseQuality::QUALITY_FAST);

    m_baseLandNoise.SetFrequency(0.0025);
    m_baseLandNoise.SetOctaveCount(5);
    m_baseLandNoise.SetPersistence(0.2);
    m_baseLandNoise.SetLacunarity(2.1);
    m_baseLandNoise.SetNoiseQuality(NoiseQuality::QUALITY_FAST);

    m_baseTerrain.SetLacunarity(1.5);
    m_baseTerrain.SetFrequency(0.01);
    m_baseTerrain.SetOctaveCount(6);
    m_baseTerrain.SetNoiseQuality(NoiseQuality::QUALITY_FAST);
}

void TerrainNoise::initTerrainModules() {
    m_flatTerrain.SetSourceModule(0, m_baseTerrain);
    m_flatTerrain.SetBias(70);
    m_flatTerrain.SetScale(5);

    m_desertTerrain.SetSourceModule(0, m_baseTerrain);
    m_desertTerrain.SetBias(75);
    m_desertTerrain.SetScale(5);

    m_beachTerrain.SetSourceModule(0, m_baseTerrain);
    m_beachTerrain.SetBias(67);
    m_beachTerrain.SetScale(4);

    m_oceanMoutainsBase.SetFrequency(0.01);
    m_oceanMoutainsBase.SetOctaveCount(5);
    m_oceanMoutainsBase.SetLacunarity(1.5);

    m_oceanMountains.SetSourceModule(0, m_oceanMoutainsBase);
    m_oceanMountains.SetScale(-0.5);
    m_oceanMountains.SetBias(1);

    m_oceanMoutainsTerrain.SetSourceModule(0, m_baseTerrain);
    m_oceanMoutainsTerrain.SetSourceModule(1, m_oceanMountains);

    m_oceanTerrain.SetSourceModule(0, m_oceanMoutainsTerrain);
    m_oceanTerrain.SetBias(30);
    m_oceanTerrain.SetScale(5);
}

void TerrainNoise::initSelectorModules() {
    m_oceanSelect.SetSourceModule(0, m_beachTerrain);
    m_oceanSelect.SetSourceModule(1, m_oceanTerrain);
    m_oceanSelect.SetSourceModule(2, m_baseNoise);
    m_oceanSelect.SetBounds(0.1, 1);
    m_oceanSelect.SetEdgeFalloff(5);

    m_desertSelect.SetSourceModule(0, m_flatTerrain);
    m_desertSelect.SetSourceModule(1, m_desertTerrain);
    m_desertSelect.SetSourceModule(2, m_baseLandNoise);
    m_desertSelect.SetBounds(-1, 0);
    m_desertSelect.SetEdgeFalloff(5);

    m_terrainSelect.SetSourceModule(0, m_oceanSelect);
    m_terrainSelect.SetSourceModule(1, m_desertSelect);
    m_terrainSelect.SetSourceModule(2, m_baseNoise);
    m_terrainSelect.SetBounds(-1, 0);
    m_terrainSelect.SetEdgeFalloff(5);    
}

int TerrainNoise::getHeight(const glm::ivec2& chunk, const int cx, const int cz) const {
    return (int)m_terrainSelect.GetValue(chunk.x * Configuration::CHUNK_SIZE + cx, 0, chunk.y * Configuration::CHUNK_SIZE + cz);
}

BiomeId TerrainNoise::getBiome(const glm::ivec2& chunk, const int cx, const int cz) const {
    int x = chunk.x * Configuration::CHUNK_SIZE + cx;
    int z = chunk.y * Configuration::CHUNK_SIZE + cz;
    
    double baseValue = m_baseNoise.GetValue(x, 0, z);
    if(baseValue < 0) {
        double landValue = m_baseLandNoise.GetValue(x, 0, z);
        if(landValue < 0.3) {
            return BiomeId::BIOME_FLAT_TERRAIN;
        }
        else {
            return BiomeId::BIOME_DESERT;
        }
    }
    else if (baseValue < 0.1) {
        return BiomeId::BIOME_BEACH;
    }
    else {
        return BiomeId::BIOME_OCEAN;
    }
}