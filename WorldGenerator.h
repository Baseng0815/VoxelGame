#pragma once

#include "pch.h"
#include "mapGenerator.h"

// Forward-Deklaration ist möglich, weil ein Zeiger immer 4 byte beansprucht und
// der compiler die Klasse daher nicht kennen muss
class World;

enum WorldType {
	WORLD_FLAT,
	WORLD_NORMAL
};

class WorldGenerator {
private:
	WorldType m_type;
	MapGenerator* map;

public:
	void init(WorldType worldType);

	void generateTerrain(World* world);
	void generateOres(World* world);
};

