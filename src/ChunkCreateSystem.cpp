#include "../include/ChunkCreateSystem.h"

#include "../include/Event.h"
#include "../include/SystemManager.h"

void ChunkCreateSystem::updateChunkBlocks(ChunkComponent& chunk) {
    chunk.blocks = new Block**[Definitions::CHUNK_SIZE];
    for (int x = 0; x < Definitions::CHUNK_SIZE; x++) {
        chunk.blocks[x] = new Block*[Definitions::CHUNK_HEIGHT];
        for (int y = 0; y < Definitions::CHUNK_HEIGHT; y++) {
            chunk.blocks[x][y] = new Block[Definitions::CHUNK_SIZE];
            for (int z = 0; z < Definitions::CHUNK_SIZE; z++)
                chunk.blocks[x][y][z] = Block(BlockType::BLOCK_AIR);
        }
    }
}

void ChunkCreateSystem::updateChunkVertices(ChunkComponent& chunk, GeometryComponent& geometry) {
    int faceCount = 0;
    int faceCountPerPass = 0;

    for (int x = 0; x < Definitions::CHUNK_SIZE; x++)
        for (int y = 0; y < Definitions::CHUNK_HEIGHT; y++)
            for (int z = 0; z < Definitions::CHUNK_SIZE; z++) {
                if (m_blocks[x][y][z].type == BlockType::BLOCK_AIR) continue;

                glm::vec3 blockPosition = glm::vec3(x, y, z);
                faceCountPerPass = 0;
                bool draw = false;

                BlockUVs blockUVs = (*m_blockUVArray)[(int)m_blocks[x][y][z].type];

                // negative X
                if (x > 0) {
                    if (m_blocks[x - 1][y][z].type == BlockType::BLOCK_AIR) draw = true;
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
                    if (m_blocks[x + 1][y][z].type == BlockType::BLOCK_AIR) draw = true;
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
                    if (m_blocks[x][y - 1][z].type == BlockType::BLOCK_AIR) draw = true;
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
                    if (m_blocks[x][y + 1][z].type == BlockType::BLOCK_AIR) draw = true;
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
                    if (m_blocks[x][y][z - 1].type == BlockType::BLOCK_AIR) draw = true;
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
                    if (m_blocks[x][y][z + 1].type == BlockType::BLOCK_AIR) draw = true;
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
    std::lock_guard<std::mutex> vaoLock(chunk.vaoMutex);
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

void ChunkCreateSystem::handleChunkEnter(ChunkEnterEvent* e) {
    EnterChunkEvent event = *e->get<EnterChunkEvent>();
    int dx = event.newX - event.oldX;
    int dz = event.newZ - event.oldZ;

    entt::registry& registry = m_systemManager->getRegistry();
    auto view = registry.view<TransformationComponent, GeometryComponent, ChunkComponent>();

    for (auto entity : view) {
        auto& transformation    = view.get<TransformationComponent>(entity);
        auto& geometry          = view.get<GeometryComponent>(entity);
        auto& chunk             = view.get<ChunkComponent>(entity);

        // remove chunks which have gone out of range
        if (std::abs(event.newX - transformation.position.x) > Definitions::CHUNK_PRELOAD_SIZE ||
            std::abs(event.newZ - transformation.position.z) > Definitions::CHUNK_PRELOAD_SIZE)
            it->second.queueDestruction();
    }

    // create new chunks which have come into range
    int pX, pZ;
    for (int x = event.newX - Definitions::CHUNK_PRELOAD_SIZE; x <= event.newX + (int)Definitions::CHUNK_PRELOAD_SIZE; x++) {
        for (int z = event.newZ - Definitions::CHUNK_PRELOAD_SIZE; z <= event.newY + (int)Definitions::CHUNK_PRELOAD_SIZE; z++) {
            pX = x;
            pZ = z;
            if (!m_chunks.count(pos))
                m_chunks.insert(std::make_pair(pos, Chunk(pos)));
        }
    }

}

ChunkCreateSystem::ChunkCreateSystem(SystemManager* systemManager, WorldType type)
    : System(systemManager) {

}

void ChunkCreateSystem::update(int dt) {
    entt::registry& registry = m_systemManager->getRegistry();
    auto view = registry.view<TransformationComponent, GeometryComponent, ChunkComponent>();

    for (auto entity : view) {
        auto& transformation    = view.get<TransformationComponent>(entity);
        auto& geometry          = view.get<GeometryComponent>(entity);
        auto& chunk             = view.get<ChunkComponent>(entity);

        if (ChunkComponent::constructionCount < 6) {
            // create blocks
            if (!chunk.m_blocks) {
                if (!chunk.threadActiveOnSelf) {
                    Chunk::constructionCount++;
                    chunk.threadActiveOnSelf = true;
                    chunk.futures.push_back(std::async(std::launch::async, [chunk]() {
                        std::lock_guard<std::mutex> blockLock(chunk.blockMutex);

                        updateChunkBlocks(chunk);

                        Chunk::constructionCount--;
                        chunk.verticesOutdated = true;
                        chunk.threadActiveOnSelf = false;
                    }));
                }
            // update vertices
            } else if (chunk.verticesOutdated) {
                if (!chunk.threadActiveOnSelf) {
                    Chunk::constructionCount++;
                    chunk.threadActiveOnSelf = true;
                    chunk.threadActiveOnSelf = false;
                    chunk.futures.push_back(std::async(std::launch::async, [chunk]() {
                        Chunk::constructionCount++;

                        updateChunkVertices(chunk, geometry);

                        Chunk::constructionCount--;
                        chunk.verticesOutdated = false;
                        chunk.threadActiveOnSelf = false;
                    }));
                }
            }
        }

        if (chunk.buffersOutdated) {
            if (!chunk.threadActiveOnSelf) {
                updateChunkBuffers(chunk, geometry);
                chunk.buffersOutdated = false;
            }
        }
    }
}

void ChunkCreateSystem::handleEvent(Event* e) {
    if (e->type() == EventType::ENTER_CHUNK_EVENT)
        handleChunkEnter(e->get<ChunkEnterEvent>);
}

