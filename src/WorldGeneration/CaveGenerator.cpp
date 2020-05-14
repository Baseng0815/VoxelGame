#include "../include/CaveGenerator.h"

#include "../include/Configuration.h"

#include <iostream>

CaveGenerator::CaveGenerator() {
	caveNoise1.SetFrequency(1.0f / 32);
	caveNoise1.SetOctaveCount(1);
	caveNoise1.SetLacunarity(3.5f);
	caveNoise1.SetSeed(rand());

	caveNoise2.SetFrequency(1.0f / 32);
	caveNoise2.SetOctaveCount(1);
	caveNoise2.SetLacunarity(3.5f);
	caveNoise2.SetSeed(rand());

	caveNoiseProd.SetSourceModule(0, caveNoise1);
	caveNoiseProd.SetSourceModule(1, caveNoise2);

	caveNoiseScale.SetSourceModule(0, caveNoiseProd);
	caveNoiseScale.SetBias(-threshold);
	caveNoiseScale.SetScale(1.3f);

	caveTurb.SetFrequency(0.01f);
	caveTurb.SetSourceModule(0, caveNoiseScale);
	caveTurb.SetPower(50);

	caveNoise.SetSourceModule(0, caveTurb);
	caveNoise.SetScale(0.7f);
}

CaveGenerator& CaveGenerator::operator=(const CaveGenerator& generator) {
	threshold = generator.threshold;

	caveNoise1.SetFrequency(generator.caveNoise1.GetFrequency());
	caveNoise1.SetOctaveCount(generator.caveNoise1.GetOctaveCount());
	caveNoise1.SetLacunarity(generator.caveNoise1.GetLacunarity());
	caveNoise1.SetSeed(generator.caveNoise1.GetSeed());

	caveNoise2.SetFrequency(generator.caveNoise2.GetFrequency());
	caveNoise2.SetOctaveCount(generator.caveNoise2.GetOctaveCount());
	caveNoise2.SetLacunarity(generator.caveNoise2.GetLacunarity());
	caveNoise2.SetSeed(generator.caveNoise2.GetSeed());

	caveNoiseProd.SetSourceModule(0, caveNoise1);
	caveNoiseProd.SetSourceModule(1, caveNoise2);

	caveNoiseScale.SetSourceModule(0, caveNoiseProd);
	caveNoiseScale.SetBias(generator.caveNoiseScale.GetBias());
	caveNoiseScale.SetScale(generator.caveNoiseScale.GetScale());

	caveTurb.SetFrequency(generator.caveTurb.GetFrequency());
	caveTurb.SetSourceModule(0, caveNoiseScale);
	caveTurb.SetPower(generator.caveTurb.GetPower());

	caveNoise.SetSourceModule(0, caveTurb);
	caveNoise.SetScale(generator.caveNoise.GetScale());

	return *this;
}

void CaveGenerator::generate(glm::vec2 position, Block*** blocks) {
	for (int cx = 0; cx < Configuration::CHUNK_SIZE; cx++) {
		for (int cy = 0; cy < Configuration::CHUNK_HEIGHT / 2; cy++) {
			for (int cz = 0; cz < Configuration::CHUNK_SIZE; cz++) {
				int x = cx + position.x * Configuration::CHUNK_SIZE;
				int y = cy;
				int z = cz + position.y * Configuration::CHUNK_SIZE;

				float caveNoise = this->caveNoise.GetValue(x, y, z);

				if (caveNoise > threshold) {
					if (blocks[cx][cy][cz].type != BlockType::BLOCK_BRICKS)
						blocks[cx][cy][cz] = Block(BlockType::BLOCK_AIR);

				}
			}
		}
	}
}