#pragma once

#include "Block.h"
#include "Vertex.h"
#include "BlockShader.h"
#include "TextureAtlas.h"

#include <mutex>
#include <future>
#include <atomic>

#if defined(WIN32)
#include <thread>
#else
#include <threads.h>
#endif

class WorldGenerator;

// TODO remove and completely replace with ECS
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
    std::atomic_bool m_threadActiveOnSelf = { false };

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
    static const WorldGenerator* m_worldGenerator;

    // keep track of how many chunks are loading at the same time and limit this number
    static std::atomic_int constructionCount;

    // keep a hold on futures because they will join on destruction
    // -> loss of benefits
    std::vector<std::future<void>> m_futures;
    glm::vec2 m_position;

    bool m_isDestructing = false;

public:
    static void setBlockUVsArray(const BlockUVsArray* array);
    static void setWorldGenerator(const WorldGenerator* generator);
    static bool positionInRange(glm::vec3 position);

    Chunk(glm::vec2 position = glm::vec2(0, 0));
    Chunk(const Chunk& other);

    // creates blocks, handles vertex and buffer updates as needed
    void update();

    void cleanUp();

    void queueDestruction();
    bool destructionReady() const;

    Block getBlock(int x, int y, int z) const;
    void setBlock(int x, int y, int z, Block block);

    void render(const BlockShader& shader, glm::vec2 position) const;
};
