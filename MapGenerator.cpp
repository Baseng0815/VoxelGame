#include "pch.h"
#include "mapGenerator.h"
#include "Chunk.h"

void MapGenerator::init(int minHeight, int maxHeight)
{
	this->minHeight = minHeight;
	this->maxHeight = maxHeight;
}

int** MapGenerator::generateMap(World* world) {
 	int** map = new int* [width];

	for (int x = 0; x < width; x++)
	{
		map[x] = new int[height];
		for (int y = 0; y < height; y++) {
			float noise = 0.0f;
			float scale = 1.0f;
			float scaleAcc = 0.0f;

			for (int o = 0; o < octavesCount; o++) {
				int pitch = width >> o;
				int sampleX1 = (x / pitch) * pitch;
				int sampleY1 = (y / pitch) * pitch;

				int sampleX2 = (sampleX1 + pitch) % width;
				int sampleY2 = (sampleY1 + pitch) % width;

				float blendX = (float)(x - sampleX1) / (float)pitch;
				float blendY = (float)(y - sampleY1) / (float)pitch;

				float sampleT = (1.0f - blendX) * seed[sampleY1 * width + sampleX1] + blendX * seed[sampleY1 * width + sampleX2];
				float sampleB = (1.0f - blendX) * seed[sampleY2 * width + sampleX1] + blendX * seed[sampleY2 * width + sampleX2];

				noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
				scaleAcc += scale;
				scale /= 2;
			}

			map[x][y] = noise * (maxHeight - minHeight) + minHeight;
		}
	}

	return map;
}

void MapGenerator::generateSeed() {
	seed = new float[height * width];

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++) {
			seed[y * width + x] = (float)rand() / (float)RAND_MAX;
		}	
}

void MapGenerator::setMapSize(int width, int height) {
	this->width = width * Chunk::CHUNK_SIZE;
	this->height = height * Chunk::CHUNK_SIZE;

	generateSeed();
}
