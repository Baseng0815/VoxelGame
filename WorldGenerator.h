#pragma once

#include "pch.h"
#include <noise/noise.h>

using namespace noise;
using namespace noise::module;

// Forward-Deklaration ist möglich, weil ein Zeiger immer 4 byte beansprucht und
// der compiler die Klasse daher nicht kennen muss
class World;

enum class WorldType : char {
	WORLD_FLAT,
	WORLD_NORMAL
};

class WorldGenerator {
private:
	WorldType m_type;
	Perlin perlinNoise;	
	void generateTerrain(glm::vec2 position, Chunk* chunk) const;
	void generateOres(const World& world, Chunk& chunk);

public:
	void init(WorldType worldType);

	void generateChunk(glm::vec2 position, Chunk* chunk) const;
};

