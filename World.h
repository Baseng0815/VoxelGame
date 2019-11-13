#pragma once

#include "pch.h"

#include "Chunk.h"
#include "WorldGenerator.h"

class World {
private:	
	Chunk m_chunks[Definitions::CHUNK_PRELOAD_SIZE][Definitions::CHUNK_PRELOAD_SIZE];

	std::array<BlockData, (int)BlockType::NUM_BLOCKS> BLOCK_DATA;

	WorldGenerator m_generator;

	friend class MasterRenderer;
	friend class WorldGenerator;
public:
	void init(WorldType worldType = WorldType::WORLD_FLAT);
	~World();

	Block getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, Block block);
	int getTerrainHeight(int x, int y);	
};
