#pragma once

#include "Chunk.h"

class ChunkWrapper {
private:
	std::mutex m_chunkMutex;
	Chunk*** m_chunks = nullptr;

	// chunks to be deleted
	std::vector<Chunk*> m_trashChunks;

	int m_x, m_z;

public:
	ChunkWrapper();

	void setCenter(int nx, int nz, bool force = false);

	Chunk* getChunkChunkSpace(int x, int z);
	Chunk* getChunkGlobalSpace(int x, int z);
};
