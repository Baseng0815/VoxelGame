#include "../include/ChunkCreateSystem.h"

#include "../include/Block.h"
#include "../include/SharedContext.h"
#include "../include/SystemManager.h"

#include "../include/Utility.h"
#include "../include/Definitions.h"
#include "../include/EventDispatcher.h"

#include "../include/Components/ChunkComponent.h"
#include "../include/Components/GeometryComponent.h"
#include "../include/Components/TransformationComponent.h"

#include <functional>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

void ChunkCreateSystem::handleEnterChunk(Event* e) {
    EnterChunkEvent event = *e->get<EnterChunkEvent>();

    // remove old chunks from ECS system and queue chunk data deletion
    auto view = m_systemManager->getRegistry().view<TransformationComponent, GeometryComponent, ChunkComponent>();

    for (auto entity : view) {
        auto& chunk          = view.get<ChunkComponent>(entity);

        if (std::abs(event.newX - chunk.chunkX) >
                Definitions::CHUNK_PRELOAD_SIZE ||
                std::abs(event.newZ - chunk.chunkZ) >
                Definitions::CHUNK_PRELOAD_SIZE) {

            if (!std::count(m_destructionQueue.begin(), m_destructionQueue.end(), entity)) {
                m_destructionQueue.push_back(entity);
            }

        }
    }

    // create new chunks which have come into range
    for (int x = event.newX + Definitions::CHUNK_PRELOAD_SIZE; x >= event.newX - (int)Definitions::CHUNK_PRELOAD_SIZE; x--) {
        for (int z = event.newZ + Definitions::CHUNK_PRELOAD_SIZE; z >= event.newZ - (int)Definitions::CHUNK_PRELOAD_SIZE; z--) {
            glm::vec2 pos(x, z);
            if (std::count(loadedChunks.begin(), loadedChunks.end(), pos) == 0) {
                auto& registry = m_systemManager->getRegistry();
                auto entity = registry.create();
                registry.emplace<TransformationComponent>(entity, glm::vec3(x * Definitions::CHUNK_SIZE,
                            0, z * Definitions::CHUNK_SIZE));
                registry.emplace<GeometryComponent>(entity);
                registry.emplace<ChunkComponent>(entity, x, z);

                loadedChunks.push_back(pos);
            }
        }
    }
}

void ChunkCreateSystem::updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ) {
    Block*** blocks = new Block**[Definitions::CHUNK_SIZE];
    for (int x = 0; x < Definitions::CHUNK_SIZE; x++) {
        blocks[x] = new Block*[Definitions::CHUNK_HEIGHT];
        for (int y = 0; y < Definitions::CHUNK_HEIGHT; y++) {
            blocks[x][y] = new Block[Definitions::CHUNK_SIZE];
            for (int z = 0; z < Definitions::CHUNK_SIZE; z++)
                blocks[x][y][z] = Block(BlockType::BLOCK_STONE);
        }
    }

    std::scoped_lock<std::mutex> blockMapLock(m_blockMapMutex);
    m_finishedBlocks.insert(std::make_pair(entity, blocks));
}

void ChunkCreateSystem::updateChunkVertices(entt::entity entity, Block*** blocks, std::mutex* blockMutex) {
    int faceCount = 0;
    int faceCountPerPass = 0;

    GeometryData geometryData;

    for (int x = 0; x < Definitions::CHUNK_SIZE; x++)
        for (int y = 0; y < Definitions::CHUNK_HEIGHT; y++)
            for (int z = 0; z < Definitions::CHUNK_SIZE; z++) {
                std::scoped_lock<std::mutex> blockLock(*blockMutex);
                if (blocks[x][y][z].type == BlockType::BLOCK_AIR) continue;

                glm::vec3 blockPosition = glm::vec3(x, y, z);
                faceCountPerPass = 0;
                bool draw = false;

                BlockUVs blockUVs = (*m_context->textureAtlas.getBlockTextureCoordinates())[(int)blocks[x][y][z].type];


                // negative X
                if (x > 0) {
                    if (blocks[x - 1][y][z].type == BlockType::BLOCK_AIR) draw = true;
                }
                else draw = true;

                if (draw) {
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][0]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][1]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][2]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][3]));

                    faceCountPerPass++;
                    draw = false;
                }

                // positive X
                if (x < Definitions::CHUNK_SIZE - 1) {
                    if (blocks[x + 1][y][z].type == BlockType::BLOCK_AIR) draw = true;
                }
                else draw = true;

                if (draw) {
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][0]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][1]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][2]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][3]));

                    faceCountPerPass++;
                    draw = false;
                }

                // negative Y
                if (y > 0) {
                    if (blocks[x][y - 1][z].type == BlockType::BLOCK_AIR) draw = true;
                }
                else draw = true;

                if (draw) {
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][0]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][1]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][2]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][3]));

                    faceCountPerPass++;
                    draw = false;
                }

                // positive Y
                if (y < Definitions::CHUNK_HEIGHT - 1) {
                    if (blocks[x][y + 1][z].type == BlockType::BLOCK_AIR) draw = true;
                }
                else draw = true;

                if (draw) {
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][0]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][1]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][2]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][3]));

                    faceCountPerPass++;
                    draw = false;
                }

                // negative Z
                if (z > 0) {
                    if (blocks[x][y][z - 1].type == BlockType::BLOCK_AIR) draw = true;
                }
                else draw = true;

                if (draw) {
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][0]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][1]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][2]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][3]));

                    faceCountPerPass++;
                    draw = false;
                }

                // positive Z
                if (z < Definitions::CHUNK_SIZE - 1) {
                    if (blocks[x][y][z + 1].type == BlockType::BLOCK_AIR) draw = true;
                }
                else draw = true;

                if (draw) {
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[1][0]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[1][1]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[1][2]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[1][3]));

                    faceCountPerPass++;
                }

                // add chunk.indices
                for (int i = 0; i < faceCountPerPass; i++) {
                    geometryData.indices.push_back(faceCount * 4 + 0);
                    geometryData.indices.push_back(faceCount * 4 + 1);
                    geometryData.indices.push_back(faceCount * 4 + 2);
                    geometryData.indices.push_back(faceCount * 4 + 0);
                    geometryData.indices.push_back(faceCount * 4 + 3);
                    geometryData.indices.push_back(faceCount * 4 + 1);

                    faceCount++;
                }
            }

    std::scoped_lock<std::mutex> geometryMapLock(m_geometryMapMutex);
    m_finishedGeometries.insert(std::make_pair(entity, geometryData));
}

void ChunkCreateSystem::updateChunkBuffers(GeometryComponent& geometry,
        const std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices) {
    if (!geometry.buffersInitialized) {
        glGenVertexArrays(1, &geometry.vao);
        glBindVertexArray(geometry.vao);
        glGenBuffers(1, &geometry.vbo);
        glGenBuffers(1, &geometry.ebo);
        glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.ebo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3) * 2));

        geometry.buffersInitialized = true;

    }

    glBindVertexArray(geometry.vao);
    glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);

    geometry.drawCount = indices.size();
}


ChunkCreateSystem::ChunkCreateSystem(SystemManager* systemManager, SharedContext* context, WorldType type)
    : System(systemManager, context), constructionCount(0) {
        ADD_EVENT(ChunkCreateSystem::handleEnterChunk, ENTER_CHUNK_EVENT);
    }

void ChunkCreateSystem::init() {

}

void ChunkCreateSystem::update(int dt) {
    // delete queued chunks if no thread is active on them
    auto it = m_destructionQueue.begin();
    while (it != m_destructionQueue.end()) {
        auto& registry = m_systemManager->getRegistry();
        auto& chunk = registry.get<ChunkComponent>(*it);

        if (!chunk.threadActiveOnSelf && chunk.blocks) {
            glm::vec2 pos(chunk.chunkX, chunk.chunkZ);

            // cleanup memory
            for (int x = 0; x < Definitions::CHUNK_SIZE; x++) {
                for (int y = 0; y < Definitions::CHUNK_HEIGHT; y++) {
                    delete[] chunk.blocks[x][y];
                }
                delete[] chunk.blocks[x];
            }

            delete[] chunk.blocks;
            delete chunk.blockMutex;

            // remove from loaded chunks and registry
            loadedChunks.erase(std::remove(loadedChunks.begin(), loadedChunks.end(), pos), loadedChunks.end());
            registry.destroy(*it);
            it = m_destructionQueue.erase(it);
        } else {
            ++it;
        }
    }

    // process finished blocks
    std::unique_lock<std::mutex> blockMapLock(m_blockMapMutex);
    for (auto const& [key, val] : m_finishedBlocks) {
        auto& chunk = m_systemManager->getRegistry().get<ChunkComponent>(key);

        chunk.blocks = val;
        chunk.verticesOutdated = true;
        chunk.threadActiveOnSelf = false;

        constructionCount--;
    }
    m_finishedBlocks.clear();
    blockMapLock.unlock();

    // process finished vertices
    std::unique_lock<std::mutex> geometryMapLock(m_geometryMapMutex);
    for (auto const& [key, val] : m_finishedGeometries) {
        auto& chunk = m_systemManager->getRegistry().get<ChunkComponent>(key);
        auto& geometry = m_systemManager->getRegistry().get<GeometryComponent>(key);

        updateChunkBuffers(geometry, val.indices, val.vertices);

        constructionCount--;
        chunk.verticesOutdated = false;
        chunk.threadActiveOnSelf = false;
    }
    m_finishedGeometries.clear();
    geometryMapLock.unlock();

    auto view = m_systemManager->getRegistry().view<TransformationComponent, GeometryComponent, ChunkComponent>();
    for (auto entity : view) {
        if (constructionCount < Definitions::CHUNK_MAX_LOADING) {
            auto& chunk             = view.get<ChunkComponent>(entity);

            if (!chunk.threadActiveOnSelf) {
                // create blocks
                if (chunk.blocks == nullptr) {
                    constructionCount++;
                    chunk.threadActiveOnSelf = true;
                    m_futures.push_back(std::async(std::launch::async, [this, &chunk, entity]() {
                        updateChunkBlocks(entity, chunk.chunkX, chunk.chunkZ);
                    }));
                // update vertices
                } else if (chunk.verticesOutdated) {
                    chunk.threadActiveOnSelf = true;
                    m_futures.push_back(std::async(std::launch::async, [this, &chunk, entity]() {
                        updateChunkVertices(entity, chunk.blocks, chunk.blockMutex);
                    }));
                }
            }
        }
    };
}
