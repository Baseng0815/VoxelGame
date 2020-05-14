#pragma once
#include "TerrainGenerator.h"
#include <noise/noise.h>

enum BiomeID;

using namespace noise;
using namespace noise::module;

class FlatTerrainGenerator : public TerrainGenerator {
private:
	Perlin perlinNoise;
	Select terrainSelector;
	Billow baseFlatTerrain;
	ScaleBias flatTerrain;

	void _init() override;
public:
	void generateTerrain(glm::vec2 chunk, Block*** blocks) override;
};
