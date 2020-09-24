#include "../../include/Systems/ChunkCreateSystem.hpp"

#include "../../include/Configuration.hpp"
#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/GameData/GameData.hpp"
#include "../../include/Math/Cuboid.hpp"
#include "../../include/World.hpp"

#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Texture.hpp"

#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Components/MeshRenderComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"

void ChunkCreateSystem::handlePlayerMoved(const EntityMovedEvent &e) {
    glm::vec2 oldChunk = Utility::GetChunk(e.oldPos);
    glm::vec2 newChunk = Utility::GetChunk(e.newPos);
    if (oldChunk == newChunk)
        return;

    int newX = newChunk.x;
    int newZ = newChunk.y;

    // remove old chunks from ECS system and queue chunk data deletion
    auto view = m_registry.view<ChunkComponent>();
    for (auto entity : view) {
        const auto &chunk = view.get<ChunkComponent>(entity);
        if (std::abs(newX - chunk.chunkX) > Configuration::CHUNK_PRELOAD_SIZE ||
            std::abs(newZ - chunk.chunkZ) > Configuration::CHUNK_PRELOAD_SIZE) {
            if (!std::count(m_destructionQueue.begin(), m_destructionQueue.end(), entity)) {
                m_destructionQueue.push_back(entity);
            }
        }
    }

    // create new chunks which have come into range
    for (int x = newX + Configuration::CHUNK_PRELOAD_SIZE; x >= newX - (int)Configuration::CHUNK_PRELOAD_SIZE; x--) {
        for (int z = newZ + Configuration::CHUNK_PRELOAD_SIZE; z >= newZ - (int)Configuration::CHUNK_PRELOAD_SIZE; z--) {
            glm::vec2 pos{x, z};
            if (std::count(m_loadedChunks.begin(), m_loadedChunks.end(), pos) == 0) {
                entt::entity entity = m_registry.create();

                const ChunkComponent &chunk = m_registry.emplace<ChunkComponent>(entity, new std::shared_mutex{}, x, z, new Geometry{});
                m_registry.emplace<TransformationComponent>(entity, glm::vec3{x * Configuration::CHUNK_SIZE, 0, z * Configuration::CHUNK_SIZE});
                m_registry.emplace<MeshRenderComponent>(entity, ResourceManager::getResource<Material>(MATERIAL_CHUNK_BLOCKS), chunk.geometry);

                m_loadedChunks.push_back(pos);
            }
        }
    }
}

void ChunkCreateSystem::handleStructureCreated(const StructureCreatedEvent &e) {
    for (const auto &pair : e.data) {
        m_structureQueue.emplace_back(pair);
    }
}

GenerationData ChunkCreateSystem::updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ) {
    GenerationData generationData;
    generationData.entity = entity;

    // allocate blocks
    generationData.blocks = new Block **[Configuration::CHUNK_SIZE];
    for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
        generationData.blocks[x] = new Block *[Configuration::CHUNK_HEIGHT];
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
            generationData.blocks[x][y] = new Block[Configuration::CHUNK_SIZE]{Block{BlockId::BLOCK_AIR}};

            if (y < 63) {
                for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
                    generationData.blocks[x][y][z] = Block{BlockId::BLOCK_WATER};
                }
            }
        }
    }

    // allocate biomes
    generationData.biomes = new BiomeId *[Configuration::CHUNK_SIZE];
    for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
        generationData.biomes[z] = new BiomeId[Configuration::CHUNK_SIZE];
    }

    glm::vec2 chunk = glm::vec2{chunkX, chunkZ};
    m_worldGenerator.generate(chunk, &generationData);
    m_structureGenerator.generateStructures(chunk, &generationData);

    // if (m_structureQueue.contains(chunk)) {
    //     updateChunkStructures(chunk, generationData.blocks);
    // }

    return generationData;
}

void ChunkCreateSystem::updateChunkStructures(Block ***chunkBlocks, BlockCollection structureBlocks) const {
    for (auto [pos, type] : structureBlocks) {
        int x = pos.x, y = pos.y, z = pos.z;
        if (chunkBlocks[x][y][z].type == BlockId::BLOCK_AIR) {
            chunkBlocks[x][y][z] = Block{type};
        }
    }
}

GeometryData ChunkCreateSystem::updateChunkVertices(entt::entity entity, Block ***blocks, std::shared_mutex *blockMutex) {
    GeometryData geometryData;
    geometryData.entity = entity;
    // reserve some space to prevent reallocations
    try {
        geometryData.vertices.reserve(1048576);
        geometryData.indices.reserve(1048576);
    }
    catch (std::length_error e) {
        std::cerr << "WARNING: chunk buffer preallocation failed; " << e.what() << std::endl;
    }

    int faceCount = 0;
    for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
                bool draw[6] = {false};

                // use shared_lock for shared read access and unique_lock for unique write access
                std::shared_lock<std::shared_mutex> blockLock(*blockMutex);
                if (blocks[x][y][z].type == BlockId::BLOCK_AIR) {
                    continue;
                }

                if (blocks[x][y][z].type == BlockId::BLOCK_WATER) {
                    if (y < Configuration::CHUNK_HEIGHT - 1 && blocks[x][y + 1][z].type == BlockId::BLOCK_AIR) {
                        draw[3] = true;
                    }
                }
                else {
                    // negative X
                    if (x > 0) {
                        if (!blocks[x - 1][y][z].isSolid())
                            draw[0] = true;
                    }
                    else
                        draw[0] = true;
                    // positive X
                    if (x < Configuration::CHUNK_SIZE - 1) {
                        if (!blocks[x + 1][y][z].isSolid())
                            draw[1] = true;
                    }
                    else
                        draw[1] = true;
                    // negative Y
                    if (y > 0) {
                        if (!blocks[x][y - 1][z].isSolid())
                            draw[2] = true;
                    }
                    else
                        draw[2] = true;
                    // positive Y
                    if (y < Configuration::CHUNK_HEIGHT - 1) {
                        if (!blocks[x][y + 1][z].isSolid())
                            draw[3] = true;
                    }
                    else
                        draw[3] = true;
                    // negative Z
                    if (z > 0) {
                        if (!blocks[x][y][z - 1].isSolid())
                            draw[4] = true;
                    }
                    else
                        draw[4] = true;
                    // positive Z
                    if (z < Configuration::CHUNK_SIZE - 1) {
                        if (!blocks[x][y][z + 1].isSolid())
                            draw[5] = true;
                    }
                    else
                        draw[5] = true;
                }
                blockLock.unlock();

                glm::vec3 blockPosition{x, y, z};
                const BlockUVs &blockUVs = m_atlas.blockUVsArray[(size_t)blocks[x][y][z].type];

                int faceCountPerPass = 0;

                // add vertices
                // TODO maybe move the values out and use loops
                if (draw[0]) {
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{0, 1, 0} + blockPosition, glm::vec3{-1, 0, 0}, blockUVs[4][0]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{0, 0, 1} + blockPosition, glm::vec3{-1, 0, 0}, blockUVs[4][1]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{0, 1, 1} + blockPosition, glm::vec3{-1, 0, 0}, blockUVs[4][2]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{0, 0, 0} + blockPosition, glm::vec3{-1, 0, 0}, blockUVs[4][3]});

                    faceCountPerPass++;
                }

                if (draw[1]) {
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{1, 1, 1} + blockPosition, glm::vec3{1, 0, 0}, blockUVs[2][0]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{1, 0, 0} + blockPosition, glm::vec3{1, 0, 0}, blockUVs[2][1]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{1, 1, 0} + blockPosition, glm::vec3{1, 0, 0}, blockUVs[2][2]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{1, 0, 1} + blockPosition, glm::vec3{1, 0, 0}, blockUVs[2][3]});

                    faceCountPerPass++;
                }

                if (draw[2]) {
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{0, 0, 1} + blockPosition, glm::vec3{0, -1, 0}, blockUVs[5][0]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{1, 0, 0} + blockPosition, glm::vec3{0, -1, 0}, blockUVs[5][1]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{1, 0, 1} + blockPosition, glm::vec3{0, -1, 0}, blockUVs[5][2]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{0, 0, 0} + blockPosition, glm::vec3{0, -1, 0}, blockUVs[5][3]});

                    faceCountPerPass++;
                }

                if (draw[3]) {
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{0, 1, 0} + blockPosition, glm::vec3{0, -1, 0}, blockUVs[0][0]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{1, 1, 1} + blockPosition, glm::vec3{0, -1, 0}, blockUVs[0][1]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{1, 1, 0} + blockPosition, glm::vec3{0, -1, 0}, blockUVs[0][2]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{0, 1, 1} + blockPosition, glm::vec3{0, -1, 0}, blockUVs[0][3]});

                    faceCountPerPass++;
                }

                if (draw[4]) {
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{1, 1, 0} + blockPosition, glm::vec3{0, 0, -1}, blockUVs[3][0]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{0, 0, 0} + blockPosition, glm::vec3{0, 0, -1}, blockUVs[3][1]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{0, 1, 0} + blockPosition, glm::vec3{0, 0, -1}, blockUVs[3][2]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{1, 0, 0} + blockPosition, glm::vec3{0, 0, -1}, blockUVs[3][3]});

                    faceCountPerPass++;
                }

                if (draw[5]) {
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{0, 1, 1} + blockPosition, glm::vec3{0, 0, -1}, blockUVs[1][0]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{1, 0, 1} + blockPosition, glm::vec3{0, 0, -1}, blockUVs[1][1]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{1, 1, 1} + blockPosition, glm::vec3{0, 0, -1}, blockUVs[1][2]});
                    geometryData.vertices.emplace_back(
                        Vertex{glm::vec3{0, 0, 1} + blockPosition, glm::vec3{0, 0, -1}, blockUVs[1][3]});

                    faceCountPerPass++;
                }

                // add indices reserve some space to prevent reallocations
                for (int i = 0; i < faceCountPerPass; i++) {
                    constexpr int offsets[] = {0, 1, 2, 0, 3, 1};

                    for (int j = 0; j < 6; j++) {
                        geometryData.indices.emplace_back(faceCount * 4 + offsets[j]);
                    }

                    faceCount++;
                }
            }
        }
    }

    return geometryData;
}

void ChunkCreateSystem::updateChunkBuffers(Geometry *geometry, const std::vector<unsigned int> &indices,
                                           const std::vector<Vertex> &vertices) {

    geometry->fillBuffers(vertices, indices);
}

void ChunkCreateSystem::_update(int dt) {
    // delete queued chunks if no thread is active on them
    auto it = m_destructionQueue.begin();
    while (it != m_destructionQueue.end()) {
        const ChunkComponent &chunk = m_registry.get<ChunkComponent>(*it);

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

            for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
                delete[] chunk.biomes[x];
            }

            delete[] chunk.biomes;

            delete chunk.blockMutex;
            delete chunk.geometry;

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
            auto &chunk = view.get<ChunkComponent>(entity);

            if (!chunk.threadActiveOnSelf) {
                // create blocks
                if (!chunk.blocks) {
                    m_constructionCount++;
                    chunk.threadActiveOnSelf = true;

                    m_generationFutures.push_back(std::async(
                        std::launch::async, [=, this]() { return updateChunkBlocks(entity, chunk.chunkX, chunk.chunkZ); }));

                    chunk.verticesOutdated = true;
                }
                // update vertices
                else if (chunk.verticesOutdated) {
                    m_constructionCount++;
                    chunk.threadActiveOnSelf = true;

                    m_geometryFutures.push_back(std::async(std::launch::async, [=, this]() {
                        return updateChunkVertices(entity, chunk.blocks, chunk.blockMutex);
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
            ChunkComponent &chunk = m_registry.get<ChunkComponent>(generationData.entity);

            chunk.blocks = generationData.blocks;
            chunk.biomes = generationData.biomes;
            // chunk.verticesOutdated = true;
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
            ChunkComponent &chunk = m_registry.get<ChunkComponent>(geometryData.entity);
            updateChunkBuffers(chunk.geometry, geometryData.indices, geometryData.vertices);

            chunk.verticesOutdated = false;
            chunk.threadActiveOnSelf = false;
            m_constructionCount--;

            itGeo = m_geometryFutures.erase(itGeo);
        }
        else {
            itGeo++;
        }
    }

    // process structures
    auto itStr = m_structureQueue.begin();
    while (itStr != m_structureQueue.end()) {
        glm::vec2 chunkPos = (*itStr).first;
        if (World::chunkCreated(chunkPos)) {
            ChunkComponent &chunk = m_registry.get<ChunkComponent>(World::getChunk(chunkPos));
            if (chunk.blocks) {
                updateChunkStructures(chunk.blocks, (*itStr).second);

                chunk.verticesOutdated = true;
                itStr = m_structureQueue.erase(itStr);
            }
            else {
                itStr++;
            }
        }
        else {
            itStr++;
        }
    }
}

ChunkCreateSystem::ChunkCreateSystem(Registry_T &registry, const TextureAtlas &atlas)
    : System{registry, 10}, m_worldGenerator{WorldType::WORLD_NORMAL}, m_structureGenerator{&m_worldGenerator}, m_atlas{atlas} {
    // event callbacks
    m_playerMovedHandle = EventDispatcher::onEntityMoved.subscribe(
        [&](const EntityMovedEvent &e) {
            if (m_registry.any<PlayerComponent>(e.entity)) {
                handlePlayerMoved(e);
            }
        });

    m_structureCreatedHandle = EventDispatcher::onStructureCreated.subscribe(
        [&](const StructureCreatedEvent &e) { handleStructureCreated(e); });

    handlePlayerMoved(EntityMovedEvent{nullptr, entt::null, glm::vec3{0.f}, glm::vec3{FLT_MAX}});
}

int ChunkCreateSystem::getActiveChunkCount() const {
    return m_loadedChunks.size();
}
