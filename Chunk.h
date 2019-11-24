#pragma once

#include "Block.h"
#include "Vertex.h"
#include "BlockShader.h"
#include "TextureAtlas.h"

class Chunk {
private:
	// 16*16*256 array of blocks
	Block*** m_blocks = nullptr;

	// deleted copy constructor because mutexes are only movable
	// maybe write one manually?
	mutable std::mutex m_blockMutex;

	GLuint m_vao;
	GLuint m_vbo, m_ebo;
	mutable std::mutex m_vaoMutex;
	unsigned int m_drawCount;
	bool m_buffersInitialized = false;

	bool m_verticesOutdated = false;
	bool m_buffersOutdated = false;
	std::atomic_bool m_threadActiveOnSelf = false;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	// allocate memory and generate blocks
	void createBlocks();

	// split up to enable multithreaded generation
	// update the geometry
	void updateVertices();

	// update the buffers if needed
	void updateBuffers();

	static const BlockUVsArray* m_blockUVArray;

	// keep a hold on futures because they will join on destruction
	// -> loss of benefits
	std::vector<std::future<void>> m_futures;

public:
	static void setBlockUVsArray(const BlockUVsArray* array);

	Chunk();
	Chunk(const Chunk& other);

	// creates blocks, handles vertex and buffer updates as needed
	void update();

	void cleanUp();

	Block getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, Block block);

	void render(const BlockShader& shader, glm::vec2 position) const;
};
