#pragma once
#include "Terrain.h"

enum BiomeID : char {
	BIOME_FLAT,
	BIOME_FLAT_TERRAIN,
	NUM_BIOMES
};

struct Biome {
	BiomeID id;
	Terrain* generator;
	
};