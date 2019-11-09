#pragma once

#include "pch.h"

#include "Chunk.h"

enum WorldType {
	WORLD_FLAT
};

// world size fixed to 7*7 chunks for now
class World {
private:
	std::array<std::array<Chunk, 7>, 7> m_chunks;

	friend class MasterRenderer;

	void generateChunk(int x, int z, WorldType type);

public:
	void init(WorldType worldType = WORLD_FLAT);

	Block getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, Block block);
};
