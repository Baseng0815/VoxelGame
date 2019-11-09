#pragma once
class World;

class MapGenerator
{
private:	
	int interval = 16;
	int octaves = 5;

	void interpolate(float** map, int interval);

public:
	int maxHeight = 0;
	int minHeight = 0;

	void init(int minHeight, int maxHeight);

	int** generateMap(World* world, int width, int height);
};

