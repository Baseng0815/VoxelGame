#include "../include/CaveGenerator.h"
#include "../include/Definitions.h"

#include <iostream>

CaveGenerator::CaveGenerator() {
	caveNoise1.SetFrequency(1.0f / 64);
	caveNoise1.SetOctaveCount(1);
	caveNoise1.SetLacunarity(3.5f);
	caveNoise1.SetSeed(rand());

	caveNoise2.SetFrequency(1.0f / 64);
	caveNoise2.SetOctaveCount(1);
	caveNoise2.SetLacunarity(3.5f);
	caveNoise2.SetSeed(rand());

	caveNoiseProd.SetSourceModule(0, caveNoise1);
	caveNoiseProd.SetSourceModule(1, caveNoise2);

	caveTurb.SetFrequency(0.01f);
	caveTurb.SetSourceModule(0, caveNoiseProd);
	caveTurb.SetPower(50);
}

CaveGenerator& CaveGenerator::operator=(const CaveGenerator& generator) {
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

	caveTurb.SetFrequency(generator.caveTurb.GetFrequency());
	caveTurb.SetSourceModule(0, caveNoiseProd);
	caveTurb.SetPower(generator.caveTurb.GetPower());

	return *this;
}

void CaveGenerator::generate(glm::vec2 position, Block*** blocks) {
	for (int cx = 0; cx < Definitions::CHUNK_SIZE; cx++) {
		for (int cy = 0; cy < Definitions::CHUNK_HEIGHT / 2; cy++) {
			for (int cz = 0; cz < Definitions::CHUNK_SIZE; cz++) {
				int x = cx + position.x * Definitions::CHUNK_SIZE;
				int y = cy;
				int z = cz + position.y * Definitions::CHUNK_SIZE;

				float caveNoise = caveTurb.GetValue(x, y, z);

				//std::cout << caveNoise << std::endl;

				if (caveNoise > threshold) {
					blocks[cx][cy][cz] = Block::Block(BlockType::BLOCK_STONE);
				}
			}
		}
	}
}