#include "../../include/Systems/ChunkCreateSystem.h"

#include "../../include/Block.h"

#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"
#include "../../include/Math/Cuboid.h"
#include "../../include/Resources/ResourceManager.h"
#include "../../include/Utility.h"
#include "../../include/World.h"

#include "../../include/Components/AtlasComponent.h"
#include "../../include/Components/ChunkComponent.h"
#include "../../include/Components/MeshRenderComponent.h"
#include "../../include/Components/TransformationComponent.h"

#include <iostream>

void ChunkCreateSystem::handleEnterChunk(const EnterChunkEvent& e) {
    // remove old chunks from ECS system and queue chunk data deletion
    auto view = m_registry.view<ChunkComponent>();

    for (auto entity : view) {
        const auto& chunk = view.get<ChunkComponent>(entity);
        if (std::abs(e.newX - chunk.chunkX) > Configuration::CHUNK_PRELOAD_SIZE ||
            std::abs(e.newZ - chunk.chunkZ) > Configuration::CHUNK_PRELOAD_SIZE) {
            if (!std::count(m_destructionQueue.begin(), m_destructionQueue.end(), entity)) {
                m_destructionQueue.push_back(entity);
            }
        }
    }

    // create new chunks which have come into range
    for (int x = e.newX + Configuration::CHUNK_PRELOAD_SIZE; x >= e.newX - (int)Configuration::CHUNK_PRELOAD_SIZE;
         x--) {
        for (int z = e.newZ + Configuration::CHUNK_PRELOAD_SIZE; z >= e.newZ - (int)Configuration::CHUNK_PRELOAD_SIZE;
             z--) {
            glm::vec2 pos{x, z};
            if (std::count(m_loadedChunks.begin(), m_loadedChunks.end(), pos) == 0) {
                entt::entity entity = m_registry.create();

                m_registry.emplace<TransformationComponent>(
                    entity, TransformationComponent{
                                glm::vec3(x * Configuration::CHUNK_SIZE, 0, z * Configuration::CHUNK_SIZE)});
                m_registry.emplace<MeshRenderComponent>(
                    entity, MeshRenderComponent{ResourceManager::getResource<Material>(MATERIAL_CHUNK_BLOCKS)});
                m_registry.emplace<ChunkComponent>(entity, ChunkComponent{new std::shared_mutex{}, x, z});

                m_loadedChunks.push_back(pos);
            }
        }
    }
}

void ChunkCreateSystem::handleBlockChanged(const BlockChangedEvent& e) {
    glm::vec3 localPos;
    glm::vec2 chunkPos = GetChunk(e.position, localPos);

    ChunkComponent& chunkComponent = m_registry.get<ChunkComponent>(World::getChunk(chunkPos));
    chunkComponent.verticesOutdated = true;
}

GenerationData ChunkCreateSystem::updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ) {
    GenerationData generationData;
    generationData.entity = entity;

    // allocate blocks
    generationData.blocks = new Block**[Configuration::CHUNK_SIZE];
    for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
        generationData.blocks[x] = new Block*[Configuration::CHUNK_HEIGHT];
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
            generationData.blocks[x][y] = new Block[Configuration::CHUNK_SIZE];

            if (y < 63) {
                for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
                    generationData.blocks[x][y][z] = Block{BlockType::BLOCK_WATER};
                }
            }
        }
    }

    // allocate biomes
    generationData.biomes = new BiomeID*[Configuration::CHUNK_SIZE];
    for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
        generationData.biomes[z] = new BiomeID[Configuration::CHUNK_SIZE];
    }

    // TODO pass generationData into this function
    m_generator.generate(glm::vec2{chunkX, chunkZ}, &generationData);

    return generationData;
}

GeometryData ChunkCreateSystem::updateChunkVertices(entt::entity entity, Block*** blocks, std::shared_mutex* blockMutex,
                                                    const AtlasComponent& atlas) {
    GeometryData geometryData;
    geometryData.entity = entity;
    int faceCount = 0;

    // reserve some space
    geometryData.vertices = std::vector<Vertex>();
    geometryData.vertices.reserve(1048576);    

    for (int x = 0; x < Configuration::CHUNK_SIZE; x++)
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++)
            for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {                
                bool draw[6] = {false};

                // use shared_lock for shared read access and unique_lock for unique write access
                std::shared_lock<std::shared_mutex> blockLock(*blockMutex);
                if (blocks[x][y][z].type == BlockType::BLOCK_AIR)
                    continue;

                if (blocks[x][y][z].type == BlockType::BLOCK_WATER) {
                    if (y < Configuration::CHUNK_HEIGHT - 1 && blocks[x][y + 1][z].type == BlockType::BLOCK_AIR) {
                        draw[3] = true;
                    }
                }
                else {
                    // negative X
                    if (x > 0) {
                        if (blocks[x - 1][y][z].isTransparent())
                            draw[0] = true;
                    }
                    else
                        draw[0] = true;
                    // positive X
                    if (x < Configuration::CHUNK_SIZE - 1) {
                        if (blocks[x + 1][y][z].isTransparent())
                            draw[1] = true;
                    }
                    else
                        draw[1] = true;
                    // negative Y
                    if (y > 0) {
                        if (blocks[x][y - 1][z].isTransparent())
                            draw[2] = true;
                    }
                    else
                        draw[2] = true;
                    // positive Y
                    if (y < Configuration::CHUNK_HEIGHT - 1) {
                        if (blocks[x][y + 1][z].isTransparent())
                            draw[3] = true;
                    }
                    else
                        draw[3] = true;
                    // negative Z
                    if (z > 0) {
                        if (blocks[x][y][z - 1].isTransparent())
                            draw[4] = true;
                    }
                    else
                        draw[4] = true;
                    // positive Z
                    if (z < Configuration::CHUNK_SIZE - 1) {
                        if (blocks[x][y][z + 1].isTransparent())
                            draw[5] = true;
                    }
                    else
                        draw[5] = true;
                }
                blockLock.unlock();

                glm::vec3 blockPosition{x, y, z};
                const BlockUVs& blockUVs = atlas.blockUVsArray[(int)blocks[x][y][z].type];

                int faceCountPerPass = 0;

                // add vertices

                
                //std::cout << sizeof(geometryData.vertices[0]) << std::endl;

                if (draw[0]) {
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(0, 1, 0) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][0]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(0, 0, 1) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][1]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(0, 1, 1) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][2]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(0, 0, 0) + blockPosition, glm::vec3(-1, 0, 0), blockUVs[4][3]});

                    faceCountPerPass++;
                }

                if (draw[1]) {
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(1, 1, 1) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][0]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(1, 0, 0) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][1]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(1, 1, 0) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][2]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(1, 0, 1) + blockPosition, glm::vec3(1, 0, 0), blockUVs[2][3]});

                    faceCountPerPass++;
                }

                if (draw[2]) {
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(0, 0, 1) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][0]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(1, 0, 0) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][1]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(1, 0, 1) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][2]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(0, 0, 0) + blockPosition, glm::vec3(0, -1, 0), blockUVs[5][3]});

                    faceCountPerPass++;
                }

                if (draw[3]) {
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(0, 1, 0) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][0]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(1, 1, 1) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][1]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(1, 1, 0) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][2]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(0, 1, 1) + blockPosition, glm::vec3(0, -1, 0), blockUVs[0][3]});

                    faceCountPerPass++;
                }

                if (draw[4]) {
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(1, 1, 0) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][0]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(0, 0, 0) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][1]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(0, 1, 0) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][2]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(1, 0, 0) + blockPosition, glm::vec3(0, 0, -1), blockUVs[3][3]});

                    faceCountPerPass++;
                }

                if (draw[5]) {
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(0, 1, 1) + blockPosition, glm::vec3(0, 0, -1), blockUVs[1][0]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(1, 0, 1) + blockPosition, glm::vec3(0, 0, -1), blockUVs[1][1]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(1, 1, 1) + blockPosition, glm::vec3(0, 0, -1), blockUVs[1][2]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3(0, 0, 1) + blockPosition, glm::vec3(0, 0, -1), blockUVs[1][3]});

                    faceCountPerPass++;
                }

                // add indices
                // reserve some space
                geometryData.indices.reserve(faceCountPerPass * 6);
                
                for (int i = 0; i < faceCountPerPass; i++) {
                    geometryData.indices.emplace_back(faceCount * 4 + 0);
                    geometryData.indices.emplace_back(faceCount * 4 + 1);
                    geometryData.indices.emplace_back(faceCount * 4 + 2);
                    geometryData.indices.emplace_back(faceCount * 4 + 0);
                    geometryData.indices.emplace_back(faceCount * 4 + 3);
                    geometryData.indices.emplace_back(faceCount * 4 + 1);

                    faceCount++;
                }
            }

    return geometryData;
}

void ChunkCreateSystem::updateChunkBuffers(Geometry& geometry, const std::vector<unsigned int>& indices,
                                           const std::vector<Vertex>& vertices) {

    geometry.fillBuffers(vertices, indices);
}

void ChunkCreateSystem::_update(int dt) {
    // delete queued chunks if no thread is active on them
    auto it = m_destructionQueue.begin();
    while (it != m_destructionQueue.end()) {
        const ChunkComponent& chunk = m_registry.get<ChunkComponent>(*it);

        if (!chunk.threadActiveOnSelf && chunk.blocks) {
            glm::vec2 pos{chunk.chunkX, chunk.chunkZ};

            World::removeChunk(*it);

            // cleanup memory
            for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
                for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
                    delete[] chunk.blocks[x][y];
                }
                delete[] chunk.blocks[x];
            }

            delete[] chunk.blocks;

            // remove from loaded chunks and registry
            m_loadedChunks.erase(std::remove(m_loadedChunks.begin(), m_loadedChunks.end(), pos), m_loadedChunks.end());
            m_registry.destroy(*it);
            it = m_destructionQueue.erase(it);
        }
        else {
            ++it;
        }
    }

    // check if chunks need updating
    auto view = m_registry.view<ChunkComponent>();
    for (auto entity : view) {
        if (m_constructionCount < Configuration::CHUNK_MAX_LOADING) {
            auto& chunk = view.get<ChunkComponent>(entity);

            if (!chunk.threadActiveOnSelf) {
                // create blocks
                if (!chunk.blocks) {
                    m_constructionCount++;
                    chunk.threadActiveOnSelf = true;

                    m_generationFutures.push_back(std::async(
                        std::launch::async, [=]() { return updateChunkBlocks(entity, chunk.chunkX, chunk.chunkZ); }));
                }
                // update vertices
                else if (chunk.verticesOutdated) {
                    m_constructionCount++;
                    chunk.threadActiveOnSelf = true;

                    const AtlasComponent& atlas = m_registry.view<AtlasComponent>().raw()[0];
                    m_geometryFutures.push_back(std::async(std::launch::async, [=]() {
                        return updateChunkVertices(entity, chunk.blocks, chunk.blockMutex, atlas);
                    }));
                }
            }
        }
    }

    // process finished blocks
    auto itGen = m_generationFutures.begin();
    while (itGen != m_generationFutures.end()) {
        if (itGen->valid() && itGen->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            GenerationData generationData = itGen->get();
            ChunkComponent& chunk = m_registry.get<ChunkComponent>(generationData.entity);

            chunk.blocks = generationData.blocks;
            chunk.biomes = generationData.biomes;
            chunk.verticesOutdated = true;
            chunk.threadActiveOnSelf = false;
            m_constructionCount--;

            World::addChunk(generationData.entity, glm::vec2(chunk.chunkX, chunk.chunkZ));

            itGen = m_generationFutures.erase(itGen);
        }
        else {
            itGen++;
        }
    }

    // process finished vertices
    auto itGeo = m_geometryFutures.begin();
    while (itGeo != m_geometryFutures.end()) {
        if (itGeo->valid() && itGeo->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            GeometryData geometryData = itGeo->get();
            ChunkComponent& chunk = m_registry.get<ChunkComponent>(geometryData.entity);
            MeshRenderComponent& meshRenderer = m_registry.get<MeshRenderComponent>(geometryData.entity);
            updateChunkBuffers(meshRenderer.geometry, geometryData.indices, geometryData.vertices);

            chunk.verticesOutdated = false;
            chunk.threadActiveOnSelf = false;
            m_constructionCount--;
            
            itGeo = m_geometryFutures.erase(itGeo);
        }
        else {
            itGeo++;
        }
    }
}

ChunkCreateSystem::ChunkCreateSystem(Registry_T& registry) : System{registry, 10}, m_constructionCount{0} {
    // event callbacks
    m_enterChunkHandle =
        EventDispatcher::onEnterChunk.subscribe([&](const EnterChunkEvent& e) { handleEnterChunk(e); });

    m_blockChangeHandle =
        EventDispatcher::onBlockChange.subscribe([&](const BlockChangedEvent& e) { handleBlockChanged(e); });

    handleEnterChunk(EnterChunkEvent());
}

int ChunkCreateSystem::getActiveChunkCount() const {
    return m_loadedChunks.size();
}
