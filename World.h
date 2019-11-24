#pragma once

#include "pch.h"

#include "Chunk.h"
//#include "WorldGenerator.h"

struct HashFunction {
	size_t operator()(const glm::vec2& v) const {
		return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
	}

	bool operator()(const glm::vec2& l, const glm::vec2& r) const {
		return l.x == r.x && l.y == r.y;
	}
};

class World {
private:	
	std::unordered_map<glm::vec2, Chunk, HashFunction, HashFunction> m_chunks;

	//WorldGenerator m_generator;

	friend class MasterRenderer;
	//friend class WorldGenerator;

public:
	void init(/*WorldType worldType = WorldType::WORLD_FLAT*/);
	~World();

	// dt in milliseconds
	void update(int dt, glm::vec3 camPos);

	Block getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, Block block);
	int getTerrainHeight(int x, int y);	
};
