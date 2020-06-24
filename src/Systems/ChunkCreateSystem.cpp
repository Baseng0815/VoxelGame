#include "../../include/Systems/ChunkCreateSystem.h"

#include "../../include/Block.h"
#include "../../include/Rendering/Vertex.h"

#include "../../include/Utility.h"
#include "../../include/Configuration.h"
#include "../../include/ResourceManager.h"
#include "../../include/Events/EventDispatcher.h"

#include "../../include/Components/ChunkComponent.h"
#include "../../include/Components/AtlasComponent.h"
#include "../../include/Components/WorldComponent.h"
#include "../../include/Components/TextureComponent.h"
#include "../../include/Components/GeometryComponent.h"
#include "../../include/Components/TransformationComponent.h"

#include <mutex>
#include <iostream>

void ChunkCreateSystem::handleEnterChunk(const EnterChunkEvent& e) {
    // remove old chunks from ECS system and queue chunk data deletion
    auto view = m_registry->view<TransformationComponent, GeometryComponent, ChunkComponent>();

    for (auto entity : view) {
        auto& chunk = view.get<ChunkComponent>(entity);

        if (std::abs(e.newX - chunk.chunkX) >
                Configuration::CHUNK_PRELOAD_SIZE ||
                std::abs(e.newZ - chunk.chunkZ) >
                Configuration::CHUNK_PRELOAD_SIZE) {

            if (!std::count(m_destructionQueue.begin(), m_destructionQueue.end(), entity)) {
                m_destructionQueue.push_back(entity);
            }

        }
    }

    // create new chunks which have come into range
    for (int x = e.newX + Configuration::CHUNK_PRELOAD_SIZE; x >= e.newX - (int)Configuration::CHUNK_PRELOAD_SIZE; x--) {
        for (int z = e.newZ + Configuration::CHUNK_PRELOAD_SIZE; z >= e.newZ - (int)Configuration::CHUNK_PRELOAD_SIZE; z--) {
            glm::vec2 pos(x, z);
            if (std::count(loadedChunks.begin(), loadedChunks.end(), pos) == 0) {
                auto entity = m_registry->create();

                m_registry->emplace<TransformationComponent>(entity, glm::vec3(x * Configuration::CHUNK_SIZE,
                            0, z * Configuration::CHUNK_SIZE));
                m_registry->emplace<GeometryComponent>(entity);
                auto& chunk = m_registry->emplace<ChunkComponent>(entity, x, z);
                chunk.blockMutex = new std::mutex();
                m_registry->emplace<TextureComponent>(entity, ResourceManager::getResource<Texture>("textureAtlas"));

                loadedChunks.push_back(pos);
            }
        }
    }
}

void ChunkCreateSystem::handleBlockChanged(const BlockChangedEvent& e) {
    auto worldView = m_registry->view<WorldComponent>();
    WorldComponent& world = worldView.get<WorldComponent>(worldView.front());
    for (auto worldEntity : worldView) {
        auto& worldComponent = worldView.get<WorldComponent>(worldEntity);
        if (worldComponent.worldID == 0)
            world = worldComponent;
    }


    glm::vec3 localPos;
    glm::vec2 chunkPos = GetChunk(e.position, localPos);

    auto chunkEntity = world.getChunk(chunkPos);
    m_outdatedChunks.push_back(chunkEntity);
}

void ChunkCreateSystem::updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ) {
    Block*** blocks = new Block * *[Configuration::CHUNK_SIZE];
    for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
        blocks[x] = new Block * [Configuration::CHUNK_HEIGHT];
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
            blocks[x][y] = new Block[Configuration::CHUNK_SIZE];

            if (y < 63) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    blocks[x][y][z] = Block(BlockType::BLOCK_WATER);
                }
            }
        }
    }

    BiomeID** biomes = new BiomeID * [Configuration::CHUNK_SIZE];
    for (int z = 0; z < Configuration::CHUNK_SIZE; z++)
        biomes[z] = new BiomeID[Configuration::CHUNK_SIZE];

    m_generator.generate(glm::vec2(chunkX, chunkZ), biomes, blocks);

    GenerationData data = GenerationData();
    data.blocks = blocks;
    data.biomes = biomes;

    std::scoped_lock<std::mutex> blockMapLock(m_blockMapMutex);
    m_finishedBlocks.insert(std::make_pair(entity, data));
}

void ChunkCreateSystem::updateChunkVertices(entt::entity entity, Block*** blocks, const AtlasComponent& atlas, std::mutex* blockMutex) {
    int faceCount = 0;

    GeometryData geometryData;

    for (int x = 0; x < Configuration::CHUNK_SIZE; x++)
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++)
            for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
                bool draw[6] = { false };

                std::unique_lock<std::mutex> blockLock(*blockMutex);
                if (blocks[x][y][z].type == BlockType::BLOCK_AIR) continue;

                if (blocks[x][y][z].type == BlockType::BLOCK_WATER) {
                    if (y < CHUNK_HEIGHT - 1 && blocks[x][y + 1][z].type == BlockType::BLOCK_AIR) {
                        draw[3] = true;
                    }
                }
                else {

                    // negative X
                    if (x > 0) {
                        if (blocks[x - 1][y][z].isTransparent()) draw[0] = true;
                    }
                    else draw[0] = true;
                    // positive X
                    if (x < Configuration::CHUNK_SIZE - 1) {
                        if (blocks[x + 1][y][z].isTransparent()) draw[1] = true;
                    }
                    else draw[1] = true;
                    // negative Y
                    if (y > 0) {
                        if (blocks[x][y - 1][z].isTransparent()) draw[2] = true;
                    }
                    else draw[2] = true;
                    // positive Y
                    if (y < Configuration::CHUNK_HEIGHT - 1) {
                        if (blocks[x][y + 1][z].isTransparent()) draw[3] = true;
                    }
                    else draw[3] = true;
                    // negative Z
                    if (z > 0) {
                        if (blocks[x][y][z - 1].isTransparent()) draw[4] = true;
                    }
                    else draw[4] = true;
                    // positive Z
                    if (z < Configuration::CHUNK_SIZE - 1) {
                        if (blocks[x][y][z + 1].isTransparent()) draw[5] = true;
                    }
                    else draw[5] = true;
                }
                blockLock.unlock();


                glm::vec3 blockPosition = glm::vec3(x, y, z);
                const BlockUVs& blockUVs = atlas.blockUVsArray[(int)blocks[x][y][z].type];

                int faceCountPerPass = 0;


                if (draw[0]) {
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][0]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][1]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][2]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][3]));

                    faceCountPerPass++;
                }

                if (draw[1]) {
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][0]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][1]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][2]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][3]));

                    faceCountPerPass++;
                }

                if (draw[2]) {
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][0]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][1]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][2]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][3]));

                    faceCountPerPass++;
                }

                if (draw[3]) {
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][0]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][1]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][2]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][3]));

                    faceCountPerPass++;
                }

                if (draw[4]) {
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][0]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][1]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][2]));
                    geometryData.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][3]));

                    faceCountPerPass++;
                }

                if (draw[5]) {
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

#include <iostream>

void ChunkCreateSystem::_update(int dt) {
    auto worldView = m_registry->view<WorldComponent>();

    WorldComponent& world = worldView.get(worldView.front());

    // delete queued chunks if no thread is active on them
    auto it = m_destructionQueue.begin();
    while (it != m_destructionQueue.end()) {
        auto& chunk = m_registry->get<ChunkComponent>(*it);

        if (!chunk.threadActiveOnSelf && chunk.blocks) {
            glm::vec2 pos(chunk.chunkX, chunk.chunkZ);

            world.removeChunk(*it);

            // cleanup memory
            for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
                for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
                    delete[] chunk.blocks[x][y];
                }
                delete[] chunk.blocks[x];
            }

            delete[] chunk.blocks;
            delete chunk.blockMutex;

            // remove from loaded chunks and registry
            loadedChunks.erase(std::remove(loadedChunks.begin(), loadedChunks.end(), pos), loadedChunks.end());
            m_registry->destroy(*it);
            it = m_destructionQueue.erase(it);
        }
        else {
            ++it;
        }
    }

    // process finished blocks
    std::unique_lock<std::mutex> blockMapLock(m_blockMapMutex);
    for (auto const& [key, val] : m_finishedBlocks) {
        auto& chunk = m_registry->get<ChunkComponent>(key);

        chunk.blocks = val.blocks;
        chunk.biomes = val.biomes;
        chunk.verticesOutdated = true;
        chunk.threadActiveOnSelf = false;
        chunk.chunkBlocksCreated = true;

        constructionCount--;
    }
    m_finishedBlocks.clear();
    blockMapLock.unlock();

    // process outdated chunks
    for (auto e : m_outdatedChunks) {
        auto& chunk = m_registry->get<ChunkComponent>(e);

        chunk.verticesOutdated = true;
        chunk.threadActiveOnSelf = false;
    }
    m_outdatedChunks.clear();

    // process finished vertices
    std::unique_lock<std::mutex> geometryMapLock(m_geometryMapMutex);
    for (auto const& [key, val] : m_finishedGeometries) {
        auto& chunk = m_registry->get<ChunkComponent>(key);
        auto& geometry = m_registry->get<GeometryComponent>(key);

        updateChunkBuffers(geometry, val.indices, val.vertices);

        constructionCount--;
        chunk.verticesOutdated = false;
        chunk.threadActiveOnSelf = false;
    }

    m_finishedGeometries.clear();
    geometryMapLock.unlock();

    auto view = m_registry->view<TransformationComponent, GeometryComponent, ChunkComponent>();
    for (auto entity : view) {
        if (constructionCount < Configuration::CHUNK_MAX_LOADING) {
            auto& chunk = view.get<ChunkComponent>(entity);

            if (!chunk.threadActiveOnSelf) {
                // create blocks
                if (chunk.blocks == nullptr) {
                    constructionCount++;
                    chunk.threadActiveOnSelf = true;

                    m_futures.push_back(std::async(std::launch::async, [=]() {
                        updateChunkBlocks(entity, chunk.chunkX, chunk.chunkZ);
                    }));

                    world.addChunk(entity, glm::vec2(chunk.chunkX, chunk.chunkZ));
                }
                // update vertices
                else if (chunk.verticesOutdated) {
                    chunk.threadActiveOnSelf = true;
                    m_registry->view<AtlasComponent>().each([&](auto& atlas) {
                        m_futures.push_back(std::async(std::launch::async, [=]() {
                            updateChunkVertices(entity, chunk.blocks, atlas, chunk.blockMutex);
                        }));
                    });
                }
            }
        }
    };
}

ChunkCreateSystem::ChunkCreateSystem(entt::registry* registry)
    : System(registry, 50), constructionCount(0) {

        // event callbacks
        EventDispatcher::onEnterChunk += [this](const EnterChunkEvent& e) {
            handleEnterChunk(e);
        };
        EventDispatcher::onBlockChange += [this](const BlockChangedEvent& e) {
            handleBlockChanged(e);
        };
    }
