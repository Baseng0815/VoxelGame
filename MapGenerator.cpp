#include "pch.h"
#include "mapGenerator.h"
#include "Chunk.h"

void MapGenerator::init(int minHeight, int maxHeight)
{
	this->minHeight = minHeight;
	this->maxHeight = maxHeight;
}

int** MapGenerator::generateMap(World* world, int width, int height) {

	float** terrain = new float* [width * Chunk::CHUNK_SIZE];
	float** seed = new float* [width * Chunk::CHUNK_SIZE];	

	//generate seed
	for (int x = 0; x < width * Chunk::CHUNK_SIZE; x++) {
		terrain[x] = new float[height * Chunk::CHUNK_SIZE];
		seed[x] = new float[height * Chunk::CHUNK_SIZE];
		for (int y = 0; y < height * Chunk::CHUNK_SIZE; y++) {
			if (x % interval == 0 && y % interval == 0)
				seed[x][y] = (float)rand() / RAND_MAX;
			else
				seed[x][y] = 0;

			terrain[x][y] = 0;
		}
	}

	interpolate(seed, interval);

	int interval = this->interval;

	for (int o = 0; o < octaves; o++) {
		for (int x = 0; x < width * Chunk::CHUNK_SIZE; x++)
			for (int y = 0; y < height * Chunk::CHUNK_SIZE; y++) {
				float 
			}
	}


	int** map = new int* [width * Chunk::CHUNK_SIZE];
	for (int x = 0; x < width * Chunk::CHUNK_SIZE; x++)
	{
		map[x] = new int[height * Chunk::CHUNK_SIZE];
		for (int y = 0; y < height * Chunk::CHUNK_SIZE; y++) {
			map[x][y] = terrain[x][y] * (maxHeight - minHeight) + minHeight;
		}
	}

	return map;
}

void MapGenerator::interpolate(float** map, int interval) {
	float first, next;
	for(int x = 0; x < )
}
