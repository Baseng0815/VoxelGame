#pragma once

#include "pch.h"

// Forward-Deklaration ist möglich, weil ein Zeiger immer 4 byte beansprucht und
// der compiler die Klasse daher nicht kennen muss
class World;

enum WorldType {
	WORLD_FLAT
};

class WorldGenerator {
private:
	WorldType m_type;

public:
	void init(WorldType worldType);

	void generate(World* world, int width, int height);
};

