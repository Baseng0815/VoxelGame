#include "../../include/WorldGeneration/TerrainModule.h"
#include "../../include/WorldGeneration/Terrains.h"
#include "../../include/Configuration.h"

using namespace noise::module;

BiomeID TerrainModule::BiomeNoise::getBiome(double x, double y, double z) const {
    return (BiomeID)(int)biomeSelector.GetValue(x, y, z);
}

TerrainModule::TerrainModule() : Module(GetSourceModuleCount()) {
    m_biomeNoise.perlin.SetFrequency(0.001);
	m_biomeNoise.perlin.SetOctaveCount(5);
	m_biomeNoise.perlin.SetPersistence(0.25);

	m_biomeNoise.land.SetFrequency(0.0025);
	m_biomeNoise.land.SetOctaveCount(5);
	m_biomeNoise.land.SetPersistence(0.2);
	m_biomeNoise.land.SetLacunarity(2.1);

	m_biomeNoise.desert.SetConstValue((int)BiomeID::BIOME_DESERT);
	m_biomeNoise.flat.SetConstValue((int)BiomeID::BIOME_FLAT_TERRAIN);

	m_biomeNoise.desertSelect.SetSourceModule(0, m_biomeNoise.desert);
	m_biomeNoise.desertSelect.SetSourceModule(1, m_biomeNoise.flat);
	m_biomeNoise.desertSelect.SetSourceModule(2, m_biomeNoise.land);
	m_biomeNoise.desertSelect.SetBounds(-1, 0);

	m_biomeNoise.ocean.SetConstValue((int)BiomeID::BIOME_OCEAN);
	m_biomeNoise.beach.SetConstValue((int)BiomeID::BIOME_BEACH);

	m_biomeNoise.oceanSelect.SetSourceModule(0, m_biomeNoise.beach);
	m_biomeNoise.oceanSelect.SetSourceModule(1, m_biomeNoise.ocean);
	m_biomeNoise.oceanSelect.SetSourceModule(2, m_biomeNoise.perlin);
	m_biomeNoise.oceanSelect.SetBounds(0.1, 1);


	m_biomeNoise.biomeSelector.SetSourceModule(0, m_biomeNoise.oceanSelect);
	m_biomeNoise.biomeSelector.SetSourceModule(1, m_biomeNoise.desertSelect);
	m_biomeNoise.biomeSelector.SetSourceModule(2, m_biomeNoise.perlin);

	m_biomeNoise.biomeSelector.SetBounds(-1, 0);	

    m_terrains.push_back({ BiomeID::BIOME_FLAT, new Terrain(0, 0) });
	m_terrains.push_back({ BiomeID::BIOME_FLAT_TERRAIN, new TerrainFlat() });
	m_terrains.push_back({ BiomeID::BIOME_DESERT, new TerrainDesert() });
	m_terrains.push_back({ BiomeID::BIOME_OCEAN, new TerrainOcean() });
	m_terrains.push_back({ BiomeID::BIOME_BEACH, new TerrainBeach() });    	
}

TerrainModule::~TerrainModule() {
    for(auto terrain : m_terrains)
        delete terrain.generator;
}

Biome TerrainModule::getBiome(BiomeID id) const {
	bool found = false;

	auto it = m_terrains.begin();
	do {
		if ((*it).id == id) {
			found = true;
		}
		else it++;
	} while (!found && it != m_terrains.end());

	if (it != m_terrains.end()) {
		Biome biome = *it;
		return biome;
	}

	return m_terrains[0];
}

int TerrainModule::GetSourceModuleCount() const {
    return 0;
}

double TerrainModule::GetValue(double x, double y, double z) const {
    BiomeID biome = m_biomeNoise.getBiome(x, 0, z);

    glm::ivec2 chunk = glm::ivec2(x / CHUNK_SIZE, z / CHUNK_SIZE);
    return getBiome(biome).generator->getHeight(chunk, (int)x % CHUNK_SIZE, (int)z % CHUNK_SIZE / 128.0f);
}

Biome TerrainModule::getBiome(double x, double z) const {
    return getBiome(m_biomeNoise.getBiome(x, 0, z));
}