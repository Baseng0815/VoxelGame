#pragma once

#include "BlockData.h"
#include "BiomeInfo.h"
#include "CaveGenerator.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <noise/noise.h>

using namespace noise;
using namespace noise::module;

// Forward-Deklaration ist möglich, weil ein Zeiger immer 4 byte beansprucht und
// der compiler die Klasse daher nicht kennen muss
class Block;

enum class WorldType : char {
	WORLD_FLAT,
	WORLD_NORMAL
};

class WorldGenerator {
private:
	WorldType m_type;
	std::unordered_map<BiomeID, BiomeInfo> biomes;	
	CaveGenerator m_caveGenerator;


	// terrain
	Perlin perlinNoise;
	Select terrainSelector;
	Billow baseFlatTerrain;
	ScaleBias flatTerrain;

	void generateTerrain(glm::vec2 position, Block*** blocks);
	void generateUnderground(glm::vec2 position, Block*** blocks);
	void generateOre(int x, int y, int z, int size, BlockType block, Block*** blocks) const;
	void init(WorldType worldType, int* seeds);

public:
	WorldGenerator() = default;
	WorldGenerator(const WorldGenerator&);

	WorldGenerator& WorldGenerator::operator=(const WorldGenerator&);

	void init(WorldType worldType);

	void generateBlocks(glm::vec2 position, Block*** blocks);
};

