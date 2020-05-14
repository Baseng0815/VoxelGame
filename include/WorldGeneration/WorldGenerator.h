#pragma once

#include "CaveGenerator.h"
#include "TerrainGenerator.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <noise/noise.h>
#include "../Block.h"

using namespace noise;
using namespace noise::module;

// Forward-Deklaration ist möglich, weil ein Zeiger immer 4 byte beansprucht und
// der compiler die Klasse daher nicht kennen muss
class Block;
class ChunkComponent;

enum BiomeID : char { 
	BIOME_FLAT, 
	NUM_BIOMES 
};

enum class WorldType : char {
	WORLD_FLAT,
	WORLD_NORMAL
};

class WorldGenerator {
private:
	WorldType m_type = WorldType::WORLD_FLAT;	
	CaveGenerator m_caveGenerator;
	std::unordered_map<BiomeID, TerrainGenerator> m_terrainGenerators;
		
	void generateUnderground(glm::vec2 position, Block*** blocks);
	void generateOre(int x, int y, int z, int size, BlockType block, Block*** blocks) const;	

public:
	WorldGenerator() = default;
	WorldGenerator(const WorldGenerator&);

	WorldGenerator& operator=(const WorldGenerator&);

	void init(WorldType worldType);

	void generate(glm::vec2 position, Block*** blocks);
};

