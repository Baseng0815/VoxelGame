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

	bool checkBounds(int x, int z) const;
	Chunk generateChunk(int x, int z, WorldType type);

public:
	void init(WorldType worldType = WORLD_FLAT);

	Chunk getChunk(int x, int z) const;
};
