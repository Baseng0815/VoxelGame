#include "pch.h"
#include "mapGenerator.h"
#include "Chunk.h"
#include "Definitions.h"

MapGenerator::MapGenerator() {
	width = Chunk::CHUNK_SIZE;
	seed = new float[width * width];
}
MapGenerator::~MapGenerator() {
	delete[] seed;
}

int** MapGenerator::generateMap(World* world) {
	int** map = new int* [width];

	for (int x = 0; x < width; x++)
	{
		map[x] = new int[width];
		for (int y = 0; y < width; y++) {
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
				scale /= 2.0f;
			}

			map[x][y] = (noise / scaleAcc) * (maxTerrainHeight - minTerrainHeight) + minTerrainHeight;
		}
	}

	return map;
}

void MapGenerator::generateSeed() {
	for (int y = 0; y < width; y++)
		for (int x = 0; x < width; x++) {
			seed[y * width + x] = (float)rand() / (float)RAND_MAX;
		}
}

void MapGenerator::setBounds(Chunk* chunk1, Chunk* chunk2) {
	
}

int** MapGenerator::generateChunkMap(World* world, glm::vec3 chunkPosition) {
	Chunk neightbourChunks[] = 

}
