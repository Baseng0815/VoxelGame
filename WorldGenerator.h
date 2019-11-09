#pragma once

#include "pch.h"

#include "World.h"

class WorldGenerator {
private:
	WorldType m_type;

public:
	void init(WorldType worldType);

	void generate(World* world, int width, int height);
};

