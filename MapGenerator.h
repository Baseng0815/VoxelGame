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
	MapGenerator();
	~MapGenerator();

	void generateSeed();
	void generateChunkSeed();
	
	void setBounds(Chunk* chunk1, Chunk* chunk2);	

	int** generateMap(World* world);
	int** generateChunkMap(World* world, glm::vec3 chunkPosition);
};

