#pragma once
#include "Definitions.h"

class World;
class Chunk;

class MapGenerator
{
private:	
	int octavesCount = 5;	
	float* terrainSeed = nullptr;
	int width = 0;
	int maxTerrainHeight = 60;
	int minTerrainHeight = 10;

	float noise(int x, int y);
public:
	MapGenerator();
	~MapGenerator();

	void generateSeed();	
	
	int** generateChunkMap(int x, int y);
};

