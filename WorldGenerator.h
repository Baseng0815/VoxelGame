#pragma once

#include "pch.h"
#include "mapGenerator.h"

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
	MapGenerator* map;
	void generateTerrain(World& world);
	void generateOres(const World& world, Chunk& chunk);

public:
	void init(WorldType worldType);

	void generate(World& world);
};

