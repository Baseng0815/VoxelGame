#pragma once
#include "Definitions.h"

class World;
class Chunk;

class MapGenerator
{
private:
	int interval = 16;
	int octavesCount = 5;	
	float* seed = nullptr;
	int width = 0;
	int maxTerrainHeight = 60;
	int minTerrainHeight = 10;

public:
	MapGenerator(int generationSize);
	~MapGenerator();

	void generateSeed();
	
	void setBounds(Chunk* chunk1, Chunk* chunk2);
	int** generateChunksMap(World* world);

	int** generateMap(World* world);
};

