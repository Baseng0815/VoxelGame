#include "../../include/WorldGeneration/CaveGenerator.h"

#include "../../include/Configuration.h"
#include "../../include/Block.h"

#include <iostream>

CaveGenerator::CaveGenerator() {
	caveNoise1.SetSeed(rand());
	caveNoise1.SetFrequency(0.075);
	caveNoise1.SetOctaveCount(1);
	caveNoise1.SetPersistence(0.25);	

	caveNoise2.SetSeed(rand());
	caveNoise2.SetFrequency(0.01);
	caveNoise2.SetOctaveCount(3);
	caveNoise2.SetPersistence(1.1);	

	caveNoiseSum.SetSourceModule(0, caveNoise1);
	caveNoiseSum.SetSourceModule(1, caveNoise2);

	cave.SetConstValue(1);
	solid.SetConstValue(0);

	scale.SetSourceModule(0, caveNoiseSum);	
	scale.SetScale(0.75, 1, 0.75);

	caveNoise.SetSourceModule(0, solid);
	caveNoise.SetSourceModule(1, cave);
	caveNoise.SetSourceModule(2, scale);

	caveNoise.SetBounds(0.25, 1);
}

CaveGenerator& CaveGenerator::operator=(const CaveGenerator& generator) {	
	caveNoise1.SetFrequency(generator.caveNoise1.GetFrequency());
	caveNoise1.SetOctaveCount(generator.caveNoise1.GetOctaveCount());
	caveNoise1.SetPersistence(generator.caveNoise1.GetPersistence());
	caveNoise1.SetSeed(generator.caveNoise1.GetSeed());

    caveNoise2.SetFrequency(generator.caveNoise2.GetFrequency());
    caveNoise2.SetOctaveCount(generator.caveNoise2.GetOctaveCount());
    caveNoise2.SetPersistence(generator.caveNoise2.GetPersistence());
    caveNoise2.SetSeed(generator.caveNoise2.GetSeed());

	caveNoiseSum.SetSourceModule(0, caveNoise1);
	caveNoiseSum.SetSourceModule(1, caveNoise2);

	cave.SetConstValue(1);
	solid.SetConstValue(0);

	caveNoise.SetSourceModule(0, solid);
	caveNoise.SetSourceModule(1, cave);
	caveNoise.SetSourceModule(2, caveNoiseSum);

	caveNoise.SetBounds(-1, -0.25);

	return *this;
}

void CaveGenerator::generate(glm::ivec2 position, Block*** blocks) {
	for (int cx = 0; cx < CHUNK_SIZE; cx++) {
		for (int cy = 0; cy < CHUNK_HEIGHT / 2; cy++) {
			for (int cz = 0; cz < CHUNK_SIZE; cz++) {
				int x = cx + position.x * CHUNK_SIZE;
				int y = cy;
				int z = cz + position.y * CHUNK_SIZE;

				float caveNoise = this->caveNoise.GetValue(x, y, z);

				if (caveNoise == 1) {
					/*if (blocks[cx][cy][cz].type != BlockType::BLOCK_BRICKS)
						blocks[cx][cy][cz] = Block(BlockType::BLOCK_AIR);*/
					blocks[cx][cy][cz] = Block(BlockType::BLOCK_STONE);

				}
			}
		}
	}
}
