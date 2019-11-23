#pragma once

#include "pch.h"
#include <noise/noise.h>

using namespace noise;
using namespace noise::module;

// Forward-Deklaration ist möglich, weil ein Zeiger immer 4 byte beansprucht und
// der compiler die Klasse daher nicht kennen muss
class World;
struct BiomeInfo;

enum class WorldType : char {
	WORLD_FLAT,
	WORLD_NORMAL
};

class WorldGenerator {
private:
	WorldType m_type;
	std::unordered_map<BiomeID, BiomeInfo> biomes;

	// caves
	int caveSize;
	int caveLenght;
	Perlin cavePerlin;

	// terrain
	Perlin perlinNoise;
	Select terrainSelector;
	Billow baseFlatTerrain;
	ScaleBias flatTerrain;

	void generateTerrain(glm::vec2 position, Chunk* chunk) const;
	void generateUnderground(glm::vec2 position, Chunk* chunk) const;
	void generateOre(int x, int y, int z, int size, BlockType block, Chunk* chunk) const;

public:
	void init(WorldType worldType);

	void generateChunk(glm::vec2 position, Chunk* chunk) const;
};

