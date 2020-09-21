#pragma once
#include "CaveGenerator.h"
#include "TerrainHeightGenerator.h"
#include "TerrainGenerator.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <noise/noise.h>
#include "../Block.h"

using namespace noise;
using namespace noise::module;

struct Block;
struct ChunkComponent;
struct GenerationData;

enum class WorldType : char {
	WORLD_FLAT,
	WORLD_NORMAL
};

class WorldGenerator {
private:
	WorldType m_type = WorldType::WORLD_FLAT;	

	//BiomeGenerator m_biomeGenerator;
	CaveGenerator m_caveGenerator;
	TerrainHeightGenerator m_heightGenerator;
	TerrainGenerator m_terrainGenerator;	
			
	void generateOres(BiomeID** biomes, Block*** blocks) const;	

public:
	WorldGenerator(WorldType type);
	WorldGenerator(const WorldGenerator&);	

	void init(WorldType worldType);

	void generate(glm::vec2 position, GenerationData* data);

	int getSurfaceHeight(glm::vec2 chunk, int x, int z) const;
};

