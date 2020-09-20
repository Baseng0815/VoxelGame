#include "../../include/WorldGeneration/TerrainHeightGenerator.h"
#include "../../include/Configuration.h"

TerrainHeightGenerator::TerrainHeightGenerator() {
	m_baseNoise.SetFrequency(0.001);
	m_baseNoise.SetOctaveCount(5);
	m_baseNoise.SetPersistence(0.25);
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

	// flat terrain values between 65 and 75
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

void TerrainHeightGenerator::generateChunkHeight(glm::vec2 chunkPos, int** heightMap, BiomeId** biomes) const {
	for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++)
		for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
			float x = chunkPos.x * Configuration::CHUNK_SIZE + cx;
			float z = chunkPos.y * Configuration::CHUNK_SIZE + cz;

			int heightValue = m_terrainSelect.GetValue(x, 0, z);
			float baseNoiseValue = m_baseNoise.GetValue(x, 0, z);

			if (baseNoiseValue < 0) {
				// land
				float landNoiseValue = m_baseLandNoise.GetValue(x, 0, z);
				if (landNoiseValue < 0) {
					biomes[cx][cz] = BiomeId::BIOME_FLAT_TERRAIN;
				}
				else {
					biomes[cx][cz] = BiomeId::BIOME_DESERT;
				}
			}
			else if (baseNoiseValue < 0.1) {
				biomes[cx][cz] = BiomeId::BIOME_BEACH;
			}
			else {
				biomes[cx][cz] = BiomeId::BIOME_OCEAN;
			}

			heightMap[cx][cz] = heightValue;
		}
}
