#pragma once
class World;
class Chunk;

class MapGenerator
{
private:	
	int interval = 16;
	int octavesCount = 5;	
	float* seed = nullptr;
	int maxTerrainHeight = 0;
	int minTerrainHeight = 0;
	int width = 0;
	int height = 0;	
	Chunk* chunk1 = nullptr;
	Chunk* chunk2 = nullptr;

public:	
	void setMapSize(int width, int height);
	void generateSeed();
	void init(int minHeight, int maxHeight);
	void setBounds(Chunk* chunk1, Chunk* chunk2);
	int** generateChunksMap(World* world);

	int** generateMap(World* world);
};

