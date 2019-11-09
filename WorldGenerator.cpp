#include "pch.h"

// in der .cpp kann man einbinden, was man will, da entsteht kein Problem
#include "World.h"
#include "WorldGenerator.h"

void WorldGenerator::init(WorldType worldType)
{
	m_type = worldType;
}

void WorldGenerator::generate(World* world, int width, int height)
{
	if (m_type == WORLD_FLAT) {

	}
}
