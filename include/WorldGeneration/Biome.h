#pragma once
#include "Terrain.h"

enum BiomeID : char {
	BIOME_FLAT,
	BIOME_FLAT_TERRAIN,
	BIOME_RIVER,
	BIOME_DESERT,
	BIOME_OCEAN,
	BIOME_BEACH,	
	NUM_BIOMES
};

struct Biome {
	BiomeID id;
	Terrain* generator;	
};