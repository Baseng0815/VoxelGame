#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Block.h"
#include "noise/noise.h"

using namespace noise;

class CaveGenerator {
private:
	module::RidgedMulti caveNoise1;
	module::RidgedMulti caveNoise2;
<<<<<<< HEAD
	module::Multiply caveNoiseProd;
=======
	
	module::Multiply caveNoiseProd;	
	module::ScaleBias caveNoiseScale;
>>>>>>> d4216d217b391d8ad2473fccff3b04e577f36e41

	module::Turbulence caveTurb;
	module::Turbulence turbY;
	module::Turbulence turbZ;

	module::ScaleBias caveNoise;

	float threshold = 0.8f;

public:
	CaveGenerator();

	CaveGenerator& operator=(const CaveGenerator&);

	void generate(glm::vec2 position, Block*** blocks);
};

