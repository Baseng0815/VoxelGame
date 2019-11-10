#pragma once
class World;

class MapGenerator
{
private:	
	int interval = 16;
	int octavesCount = 5;	
	float* seed = nullptr;
	int width = 0;
	int height = 0;	

public:
	int maxHeight = 0;
	int minHeight = 0;

	void setMapSize(int width, int height);
	void generateSeed();
	void init(int minHeight, int maxHeight);

	int** generateMap(World* world);
};

