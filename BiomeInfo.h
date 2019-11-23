#pragma once
#include "pch.h"

enum class BiomeID : char {
	BIOME_FLAT, NUM_BIOMES
};

struct BiomeInfo {
	int baseHeight;
	int heightAmplitude;

	BiomeID id;
};