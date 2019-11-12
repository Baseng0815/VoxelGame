#pragma once

#include "pch.h"

#include "Chunk.h"
#include "WorldGenerator.h"

// world size fixed to 7*7 chunks for now
class World {
private:
	//std::array<std::array<Chunk, 7>, 7> m_chunks;
	std::vector<std::vector<Chunk>> m_chunks;

	std::array<BlockData, (int)BlockType::NUM_BLOCKS> BLOCK_DATA;

	WorldGenerator m_generator;

	friend class MasterRenderer;
	friend class WorldGenerator;
public:
	void init(WorldType worldType = WORLD_FLAT);

	Block getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, Block block);
	int getTerrainHeight(int x, int y);
	Chunk* getChunk(int x, int y);
};
