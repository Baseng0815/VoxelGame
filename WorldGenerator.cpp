#include "pch.h"
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
