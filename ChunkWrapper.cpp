#include "pch.h"

#include "ChunkWrapper.h"

ChunkWrapper::ChunkWrapper() {
	setCenter(0, 0);
}

void ChunkWrapper::setCenter(int x, int z, bool force) {
	// create
	if (m_chunks == nullptr) {
		m_x = x;
		m_z = z;

		for (int x = 0; x < CHUNK_COUNT; x++) {
			m_chunks[x] = new Chunk*[CHUNK_COUNT];
			for (int z = 0; z < CHUNK_COUNT; z++)
				m_chunks[x][z] = new Chunk(glm::vec2(m_x - RENDER_DISTANCE + x,
					m_z - RENDER_DISTANCE + z));
		}
	}
	// move
	else {
		if (x == m_x && z == m_z)
			return;

		std::unique_lock<std::mutex> lock;

		int dX = x - m_x;
		int dZ = z - m_z;

		Chunk*** newChunks;
		for (int x = 0; x < CHUNK_COUNT; x++)
			newChunks[x] = new Chunk*[CHUNK_COUNT];
			for (int z = 0; z < CHUNK_COUNT; z++) {
				int oldX = x + dX;
				int oldZ = z + dZ;

				if (oldX > 0 && oldX < CHUNK_COUNT && oldZ > 0 && oldZ < CHUNK_COUNT)
					newChunks[x][z] = m_chunks[oldX][oldZ];
				else
					newChunks[x][z] = new Chunk(glm::vec2(m_x - RENDER_DISTANCE + x,
						m_z - RENDER_DISTANCE + z));
			}

		// remove old chunks
		for (int x = 0; x < CHUNK_COUNT; x++)
			for (int z = 0; z < CHUNK_COUNT; z++) {
				int newX = x - dX;
				int newZ = z - dZ;

				if (newX < 0 || newX >= CHUNK_COUNT || newZ < 0 || newZ >= CHUNK_COUNT)
					if (!m_chunks[x][z]->tryDelete())
						m_trashChunks.push_back(m_chunks[x][z]);
			}

		m_x = x;
		m_z = z;

		for (int i = 0; i < CHUNK_COUNT; i++)
			delete[] m_chunks[i];
		delete[] m_chunks;

		m_chunks = newChunks;
	}
}

void ChunkWrapper::update() {
	// try to delete trash chunks
	auto it = m_trashChunks.begin();
	while (it != m_trashChunks.end()) {
		auto oldIt = it;
		it++;

		// if delete is successful, erase it from list
		if ((*oldIt)->tryDelete())
			m_trashChunks.erase(oldIt);
	}
}