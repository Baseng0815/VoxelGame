#include "pch.h"
#include "mapGenerator.h"
#include "Chunk.h"
#include "Definitions.h"

void MapGenerator::init(int minHeight, int maxHeight)
{
	this->minTerrainHeight = minHeight;
	this->maxTerrainHeight = maxHeight;
}

int** MapGenerator::generateMap(World* world) {
 	int** map = new int* [Definitions::generationSize];

	for (int x = 0; x < Definitions::generationSize; x++)
	{
		map[x] = new int[Definitions::generationSize];
		for (int y = 0; y < height; y++) {
			float noise = 0.0f;
			float scale = 1.0f;
			float scaleAcc = 0.0f;

			for (int o = 0; o < octavesCount; o++) {
				int pitch = Definitions::generationSize >> o;
				int sampleX1 = (x / pitch) * pitch;
				int sampleY1 = (y / pitch) * pitch;

				int sampleX2 = (sampleX1 + pitch) % Definitions::generationSize;
				int sampleY2 = (sampleY1 + pitch) % Definitions::generationSize;

				float blendX = (float)(x - sampleX1) / (float)pitch;
				float blendY = (float)(y - sampleY1) / (float)pitch;

				float sampleT = (1.0f - blendX) * seed[sampleY1 * Definitions::generationSize + sampleX1] + blendX * seed[sampleY1 * Definitions::generationSize + sampleX2];
				float sampleB = (1.0f - blendX) * seed[sampleY2 * Definitions::generationSize + sampleX1] + blendX * seed[sampleY2 * Definitions::generationSize + sampleX2];

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
	for (int y = 0; y < Definitions::generationSize; y++)
		for (int x = 0; x < Definitions::generationSize; x++) {
			seed[y * width + x] = (float)rand() / (float)RAND_MAX;
		}		
}

void MapGenerator::setBounds(Chunk* chunk1, Chunk* chunk2) {
	this->chunk1 = chunk1;
	this->chunk2 = chunk2;

	glm::vec3 mapSize = chunk2->position - chunk1->position;

	this->width = mapSize.x + Chunk::CHUNK_SIZE;
	this->height = mapSize.z + Chunk::CHUNK_SIZE;
}

int** MapGenerator::generateChunksMap(World* world) {
	return nullptr;
}
