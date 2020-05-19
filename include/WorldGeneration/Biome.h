#pragma once
#include "Terrain.h"

enum BiomeID : char {
	BIOME_FLAT,
	BIOME_FLAT_TERRAIN,
	BIOME_DESERT,
	NUM_BIOMES
};

struct Biome {
	BiomeID id;
	Terrain* generator;	
};