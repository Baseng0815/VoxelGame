#include "pch.h"
#include "mapGenerator.h"
#include "Chunk.h"
#include "Definitions.h"

MapGenerator::MapGenerator() {
	width = Definitions::MAP_SEED_SIZE;
	terrainSeed = new float[width * width];
}
MapGenerator::~MapGenerator() {
	delete[] terrainSeed;
}

float MapGenerator::noise(int x, int y) {
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

		float sampleT = (1.0f - blendX) * terrainSeed[sampleY1 * width + sampleX1] + blendX * terrainSeed[sampleY1 * width + sampleX2];
		float sampleB = (1.0f - blendX) * terrainSeed[sampleY2 * width + sampleX1] + blendX * terrainSeed[sampleY2 * width + sampleX2];

		noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
		scaleAcc += scale;
		scale /= 2.0f;
	}

	return (noise / scaleAcc);
}

void MapGenerator::generateSeed() {
	for (int y = 0; y < width; y++)
		for (int x = 0; x < width; x++) {
			terrainSeed[y * width + x] = (float)rand() / (float)RAND_MAX;
		}
}

int** MapGenerator::generateChunkMap(int x, int y) {
	int** map = new int* [Chunk::CHUNK_SIZE];

	for (int cx = 0; cx < Chunk::CHUNK_SIZE; cx++) {
		map[cx] = new int[Chunk::CHUNK_SIZE];

		for (int cz = 0; cz < Chunk::CHUNK_SIZE; cz++) {

			float noise = this->noise(x * Chunk::CHUNK_SIZE + cx, y * Chunk::CHUNK_SIZE + cz);
			map[cx][cz] = noise * (maxTerrainHeight - minTerrainHeight) + minTerrainHeight;
		}
	}

	return map;
}
