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

void ChunkCreateSystem::handleEnterChunk(Event* e) {
    EnterChunkEvent event = *e->get<EnterChunkEvent>();

    entt::registry& registry = m_systemManager->getRegistry();
    auto view = registry.view<TransformationComponent, GeometryComponent, ChunkComponent>();

    // remove old chunks from ECS system and queue chunk data deletion
    for (auto entity : view) {
        auto& transformation = view.get<TransformationComponent>(entity);
        auto& chunk          = view.get<ChunkComponent>(entity);
        std::cout << chunk.chunkX << " " << chunk.chunkZ << std::endl;

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
    for (int x = event.newX - Definitions::CHUNK_PRELOAD_SIZE; x <= event.newX + (int)Definitions::CHUNK_PRELOAD_SIZE; x++) {
        for (int z = event.newZ - Definitions::CHUNK_PRELOAD_SIZE; z <= event.newZ + (int)Definitions::CHUNK_PRELOAD_SIZE; z++) {
            glm::vec2 pos(x, z);
            if (!std::count(loadedChunks.begin(), loadedChunks.end(), pos)) {
                auto entity = m_registry->create();
                m_registry->assign<TransformationComponent>(entity, glm::vec3(x * Definitions::CHUNK_SIZE * 1.2,
                                   0, z * Definitions::CHUNK_SIZE * 1.2));
                m_registry->assign<GeometryComponent>(entity);
                m_registry->assign<ChunkComponent>(entity, x, z);

                auto& transform = m_registry->get<TransformationComponent>(entity);

                loadedChunks.push_back(pos);
            }
        }
    }
}

void ChunkCreateSystem::updateChunkBlocks(ChunkComponent& chunk) {
    chunk.blocks = new Block**[Definitions::CHUNK_SIZE];
    for (int x = 0; x < Definitions::CHUNK_SIZE; x++) {
        chunk.blocks[x] = new Block*[Definitions::CHUNK_HEIGHT];
        for (int y = 0; y < Definitions::CHUNK_HEIGHT; y++) {
            chunk.blocks[x][y] = new Block[Definitions::CHUNK_SIZE];
            for (int z = 0; z < Definitions::CHUNK_SIZE; z++)
                chunk.blocks[x][y][z] = Block(BlockType::BLOCK_STONE);
        }
    }
}

void ChunkCreateSystem::updateChunkVertices(ChunkComponent& chunk, GeometryComponent& geometry) {
    int faceCount = 0;
    int faceCountPerPass = 0;

    for (int x = 0; x < Definitions::CHUNK_SIZE; x++)
        for (int y = 0; y < Definitions::CHUNK_HEIGHT; y++)
            for (int z = 0; z < Definitions::CHUNK_SIZE; z++) {
                if (chunk.blocks[x][y][z].type == BlockType::BLOCK_AIR) continue;

                glm::vec3 blockPosition = glm::vec3(x, y, z);
                faceCountPerPass = 0;
                bool draw = false;

                std::unique_lock<std::mutex> blockLock(chunk.blockMutex);
                BlockUVs blockUVs = (*m_context->textureAtlas.getBlockTextureCoordinates())[(int)chunk.blocks[x][y][z].type];
                blockLock.unlock();

                // negative X
                if (x > 0) {
                    if (chunk.blocks[x - 1][y][z].type == BlockType::BLOCK_AIR) draw = true;
                }
                else draw = true;

                if (draw) {
                    chunk.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][0]));
                    chunk.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][1]));
                    chunk.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][2]));
                    chunk.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][3]));

                    faceCountPerPass++;
                    draw = false;
                }

                // positive X
                if (x < Definitions::CHUNK_SIZE - 1) {
                    if (chunk.blocks[x + 1][y][z].type == BlockType::BLOCK_AIR) draw = true;
                }
                else draw = true;

                if (draw) {
                    chunk.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][0]));
                    chunk.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][1]));
                    chunk.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][2]));
                    chunk.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][3]));

                    faceCountPerPass++;
                    draw = false;
                }

                // negative Y
                if (y > 0) {
                    if (chunk.blocks[x][y - 1][z].type == BlockType::BLOCK_AIR) draw = true;
                }
                else draw = true;

                if (draw) {
                    chunk.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][0]));
                    chunk.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][1]));
                    chunk.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][2]));
                    chunk.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][3]));

                    faceCountPerPass++;
                    draw = false;
                }

                // positive Y
                if (y < Definitions::CHUNK_HEIGHT - 1) {
                    if (chunk.blocks[x][y + 1][z].type == BlockType::BLOCK_AIR) draw = true;
                }
                else draw = true;

                if (draw) {
                    chunk.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][0]));
                    chunk.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][1]));
                    chunk.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][2]));
                    chunk.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][3]));

                    faceCountPerPass++;
                    draw = false;
                }

                // negative Z
                if (z > 0) {
                    if (chunk.blocks[x][y][z - 1].type == BlockType::BLOCK_AIR) draw = true;
                }
                else draw = true;

                if (draw) {
                    chunk.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][0]));
                    chunk.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][1]));
                    chunk.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][2]));
                    chunk.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][3]));

                    faceCountPerPass++;
                    draw = false;
                }

                // positive Z
                if (z < Definitions::CHUNK_SIZE - 1) {
                    if (chunk.blocks[x][y][z + 1].type == BlockType::BLOCK_AIR) draw = true;
                }
                else draw = true;

                if (draw) {
                    chunk.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[1][0]));
                    chunk.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[1][1]));
                    chunk.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[1][2]));
                    chunk.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[1][3]));

                    faceCountPerPass++;
                }

                // add chunk.indices
                for (int i = 0; i < faceCountPerPass; i++) {
                    chunk.indices.push_back(faceCount * 4 + 0);
                    chunk.indices.push_back(faceCount * 4 + 1);
                    chunk.indices.push_back(faceCount * 4 + 2);
                    chunk.indices.push_back(faceCount * 4 + 0);
                    chunk.indices.push_back(faceCount * 4 + 3);
                    chunk.indices.push_back(faceCount * 4 + 1);

                    faceCount++;
                }
            }

    geometry.drawCount = chunk.indices.size();

}

void ChunkCreateSystem::updateChunkBuffers(ChunkComponent& chunk, GeometryComponent& geometry) {
    if (!chunk.buffersInitialized) {
        glGenVertexArrays(1, &geometry.vao);
        glBindVertexArray(geometry.vao);
        glGenBuffers(1, &geometry.vbo);
        glGenBuffers(1, &geometry.ebo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3) * 2));

        chunk.buffersInitialized = true;
    }

    glBindVertexArray(geometry.vao);
    glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(chunk.vertices[0]) * chunk.vertices.size(), chunk.vertices.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(chunk.indices[0]) * chunk.indices.size(), chunk.indices.data(), GL_DYNAMIC_DRAW);

    chunk.vertices.clear();
    chunk.indices.clear();
}


ChunkCreateSystem::ChunkCreateSystem(SystemManager* systemManager, SharedContext* context, WorldType type)
    : System(systemManager, context), constructionCount(0) {
    ADD_EVENT(ChunkCreateSystem::handleEnterChunk, ENTER_CHUNK_EVENT);
}

void ChunkCreateSystem::init() {

}

void ChunkCreateSystem::update(int dt) {
    // delete queued chunks if no thread is active on them
    for (auto it = m_destructionQueue.begin(); it != m_destructionQueue.end();) {
        auto& chunk = m_registry->get<ChunkComponent>(*it);

        if (!chunk.threadActiveOnSelf) {
            glm::vec2 pos(chunk.chunkX, chunk.chunkZ);

            // cleanup memory
            for (int x = 0; x < Definitions::CHUNK_SIZE; x++) {
                for (int y = 0; y < Definitions::CHUNK_HEIGHT; y++) {
                    delete[] chunk.blocks[x][y];
                }
                delete[] chunk.blocks[x];
            }

            delete[] chunk.blocks;

            // remove from loaded chunks and registry
            loadedChunks.erase(std::remove(loadedChunks.begin(), loadedChunks.end(), pos), loadedChunks.end());
            m_registry->destroy(*it);
            it = m_destructionQueue.erase(it);
        } else {
            it++;
        }
    }

    auto view = m_registry->view<TransformationComponent, GeometryComponent, ChunkComponent>();
    for (auto entity : view ) {

        auto& transformation = view.get<TransformationComponent>(entity);
        auto& geometry       = view.get<GeometryComponent>(entity);
        auto& chunk          = view.get<ChunkComponent>(entity);

        if (constructionCount < 6) {
            // create blocks
            if (!chunk.blocks) {
                if (!chunk.threadActiveOnSelf) {
                    constructionCount++;
                    chunk.threadActiveOnSelf = true;
                    chunk.futures.push_back(std::async(std::launch::async, [this, &chunk]() {
                        updateChunkBlocks(chunk);

                        constructionCount--;
                        chunk.verticesOutdated = true;
                        chunk.threadActiveOnSelf = false;
                    }));
                }
            // update vertices
            } else if (chunk.verticesOutdated) {
                if (!chunk.threadActiveOnSelf) {
                    constructionCount++;
                    chunk.threadActiveOnSelf = true;
                    chunk.futures.push_back(std::async(std::launch::async, [this, &chunk, &geometry]() {

                        updateChunkVertices(chunk, geometry);

                        constructionCount--;
                        chunk.threadActiveOnSelf = false;
                        chunk.verticesOutdated = false;
                        chunk.buffersOutdated = true;
                    }));
                }
            }
        }

        // keep buffers updated
        if (chunk.buffersOutdated) {
            if (!chunk.threadActiveOnSelf) {
                updateChunkBuffers(chunk, geometry);
                chunk.buffersOutdated = false;
            }
        }
    };
}
