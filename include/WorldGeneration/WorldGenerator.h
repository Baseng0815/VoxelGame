#pragma once

#include "CaveGenerator.h"
#include "TerrainModule.h"
#include "BiomeGenerator.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <noise/noise.h>
#include "../Block.h"
#include "Gradient.h"

using namespace noise;
using namespace noise::module;

// Forward-Deklaration ist m�glich, weil ein Zeiger immer 4 byte beansprucht und
// der compiler die Klasse daher nicht kennen muss
class Block;
class ChunkComponent;

enum class WorldType : char {
	WORLD_FLAT,
	WORLD_NORMAL
};

class WorldGenerator {
private:
	WorldType m_type = WorldType::WORLD_FLAT;	

	//BiomeGenerator m_biomeGenerator;
	CaveGenerator m_caveGenerator;
	TerrainModule m_terrainGenerator;
	Gradient m_terrainGradient;
			
	void generateOres(BiomeID** biomes, Block*** blocks) const;	

public:
	WorldGenerator(WorldType type);
	WorldGenerator(const WorldGenerator&);

	WorldGenerator& operator=(const WorldGenerator&);

	void init(WorldType worldType);

	void generate(glm::ivec2 position, BiomeID** biomes, Block*** blocks);
};

