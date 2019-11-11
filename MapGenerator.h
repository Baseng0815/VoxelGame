#pragma once
#include "Definitions.h"

class World;
class Chunk;

class MapGenerator
{
private:	
	int interval = 16;
	int octavesCount = 5;	
	float seed[Definitions::generationSize * Definitions::generationSize];
	int maxTerrainHeight = 0;
	int minTerrainHeight = 0;
	
	Chunk* chunk1 = nullptr;
	Chunk* chunk2 = nullptr;

public:		
	void generateSeed();
	void init(int minHeight, int maxHeight);
	void setBounds(Chunk* chunk1, Chunk* chunk2);
	int** generateChunksMap(World* world);

	int** generateMap(World* world);
};

