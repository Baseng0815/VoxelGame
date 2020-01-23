#include "../include/Chunk.h"
#include "../include/Vertex.h"
#include "../include/Utility.h"
#include "../include/WorldGenerator.h"

#define  GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

const BlockUVsArray* Chunk::m_blockUVArray;
const WorldGenerator* Chunk::m_worldGenerator;
std::atomic_int Chunk::constructionCount = { 0 };

void Chunk::createBlocks() {
    std::lock_guard<std::mutex> blockLock(m_blockMutex);

    m_blocks = new Block**[Definitions::CHUNK_SIZE];
    for (int x = 0; x < Definitions::CHUNK_SIZE; x++) {
        m_blocks[x] = new Block * [Definitions::CHUNK_HEIGHT];
        for (int y = 0; y < Definitions::CHUNK_HEIGHT; y++) {
            m_blocks[x][y] = new Block[Definitions::CHUNK_SIZE];
            for (int z = 0; z < Definitions::CHUNK_SIZE; z++)
                m_blocks[x][y][z] = Block(BlockType::BLOCK_AIR);
        }
    }

    // get blocks from terrain generation
    m_worldGenerator->generateBlocks(m_position, m_blocks);

    constructionCount--;
}

void Chunk::updateVertices() {
}

void Chunk::updateBuffers() {
    std::lock_guard<std::mutex> m_vaoLock(m_vaoMutex);
    if (!m_buffersInitialized) {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);
        m_buffersInitialized = true;
    }


}


void Chunk::setBlockUVsArray(const BlockUVsArray* array) {
    m_blockUVArray = array;
}

void Chunk::setWorldGenerator(const WorldGenerator* worldGenerator) {
    m_worldGenerator = worldGenerator;
}

Chunk::Chunk(glm::vec2 position)
    : m_position(position) {}

    Chunk::Chunk(const Chunk& other) {
        m_blocks = other.m_blocks;
        m_vao = other.m_vao;
        m_vbo = other.m_vbo;
        m_ebo = other.m_ebo;
        m_drawCount = other.m_drawCount;
        m_buffersInitialized = other.m_buffersInitialized;
        m_verticesOutdated = other.m_verticesOutdated;
        m_buffersOutdated = other.m_buffersOutdated;
        m_vertices = other.m_vertices;
        m_indices = other.m_indices;
        m_position = other.m_position;
    }

void Chunk::update() {
    if (constructionCount < Definitions::CHUNK_MAX_LOADING) {
        if (!m_blocks) {
            if (!m_threadActiveOnSelf) {
                constructionCount++;
                m_threadActiveOnSelf = true;
                m_futures.push_back(std::async(std::launch::async, [this]() {
                            createBlocks();
                            m_verticesOutdated = true;
                            m_threadActiveOnSelf = false;
                            }));
            }
        } else if (m_verticesOutdated) {
            if (!m_threadActiveOnSelf) {
                constructionCount++;
                m_threadActiveOnSelf = true;
                m_futures.push_back(std::async(std::launch::async, [this]() {
                            updateVertices();
                            m_verticesOutdated = false;
                            m_buffersOutdated = true;
                            m_threadActiveOnSelf = false;
                            }));
            }
        }
    }

    // update buffers on main thread, does not block much
    if (m_buffersOutdated) {
        if (!m_threadActiveOnSelf) {
            updateBuffers();
            m_buffersOutdated = false;
        }
    }
}

void Chunk::cleanUp() {
    if (m_blocks) {
        for (int x = 0; x < Definitions::CHUNK_SIZE; x++) {
            for (int y = 0; y < Definitions::CHUNK_HEIGHT; y++)
                delete[] m_blocks[x][y];
            delete[] m_blocks[x];
        }
        delete[] m_blocks;
    }

    if (m_buffersInitialized) {
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
        glDeleteVertexArrays(1, &m_vao);
    }
}

void Chunk::queueDestruction() {
    m_isDestructing = true;
}

bool Chunk::destructionReady() const {
    return m_isDestructing && !m_threadActiveOnSelf;
}

Block Chunk::getBlock(int x, int y, int z) const {
    if (m_blocks) {
        std::lock_guard<std::mutex> blockLock(m_blockMutex);
        return m_blocks[x][y][z];
    }
    return Block(BlockType::BLOCK_AIR);
}

void Chunk::setBlock(int x, int y, int z, Block block) {
    std::lock_guard<std::mutex> m_blockLock(m_blockMutex);
    m_blocks[x][y][z] = block;
}

void Chunk::render(const BlockShader& shader, glm::vec2 position) const {
    if (m_buffersInitialized) {
        std::lock_guard<std::mutex> m_vaoLock(m_vaoMutex);
        shader.uploadModelMatrix(glm::translate(glm::vec3(position.x * Definitions::CHUNK_SIZE, 0,
                        position.y * Definitions::CHUNK_SIZE)));
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, nullptr);
    }
}
