#pragma once

#include "pch.h"

#include "Chunk.h"
#include "WorldGenerator.h"

class World {
private:	
	std::vector<Chunk> m_chunks;

	std::array<BlockData, (int)BlockType::NUM_BLOCKS> BLOCK_DATA;

	WorldGenerator m_generator;

	friend class MasterRenderer;
	friend class WorldGenerator;

	Chunk* createChunk(int x, int y);
public:
	void init(WorldType worldType = WORLD_FLAT);

	Block getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, Block block);
	int getTerrainHeight(int x, int y);	
	Chunk* getChunk(int x, int y);
};
