#include "../../include/Systems/ChunkCreateSystem.hpp"

#include "../../include/Configuration.hpp"
#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/GameData/BlockGeometry.hpp"
#include "../../include/GameData/GameData.hpp"
#include "../../include/Math/Cuboid.hpp"
#include "../../include/Resources/TextureAtlas.hpp"
#include "../../include/World.hpp"

#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Texture.hpp"

#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Components/MultiMeshRenderComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"

void ChunkCreateSystem::handlePlayerMoved(const EntityMovedEvent& e) {
    glm::vec2 oldChunk = Utility::GetChunk(e.oldPos);
    glm::vec2 newChunk = Utility::GetChunk(e.newPos);
    if (oldChunk == newChunk)
        return;

    int newX = newChunk.x;
    int newZ = newChunk.y;

    // queue chunk deletion
    auto view = m_registry.view<ChunkComponent>();
    for (auto entity : view) {
        const auto& chunk = view.get<ChunkComponent>(entity);
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

                const ChunkComponent& chunk = m_registry.emplace<ChunkComponent>(entity, new std::shared_mutex{}, x, z, new Geometry{}, new Geometry{}, new Geometry{});
                m_registry.emplace<TransformationComponent>(entity, glm::vec3{x * Configuration::CHUNK_SIZE, 0, z * Configuration::CHUNK_SIZE});
                // TODO add two mesh renderers; one for solid blocks with culling and one for planes without culling
                m_registry.emplace<MultiMeshRenderComponent>(entity, std::vector<MeshRenderComponent>{
                                                                         {ResourceManager::getResource<Material>(MATERIAL_CHUNK_BLOCKS_CULLED), chunk.geometryCulled},
                                                                         {ResourceManager::getResource<Material>(MATERIAL_CHUNK_BLOCKS_NON_CULLED), chunk.geometryNonCulled},
                                                                         {ResourceManager::getResource<Material>(MATERIAL_WATER), chunk.geometryTransparent, 1}});

                m_loadedChunks.push_back(pos);
            }
        }
    }
}

void ChunkCreateSystem::handleStructureCreated(const StructureCreatedEvent& e) {
    for (const auto& pair : e.data) {
        // TODO: fix error
        try {
            m_structureQueue.insert(pair);
        }
        catch (std::exception) {
            // ignore
        }
    }
}

GenerationData ChunkCreateSystem::updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ) {
    GenerationData generationData;
    generationData.entity = entity;    

    // allocate blocks
    generationData.blocks = new short**[Configuration::CHUNK_SIZE];
    for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
        generationData.blocks[x] = new short*[Configuration::CHUNK_HEIGHT];
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
            generationData.blocks[x][y] = new short[Configuration::CHUNK_SIZE];
        }
    }

    // allocate biomes
    generationData.biomes = new BiomeId*[Configuration::CHUNK_SIZE];
    for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
        generationData.biomes[z] = new BiomeId[Configuration::CHUNK_SIZE];
    }

    glm::vec2 chunk = glm::vec2{chunkX, chunkZ};
    m_worldGenerator.generate(chunk, &generationData);
    // m_structureGenerator.generateStructures(chunk, &generationData);

    // if (m_structureQueue.contains(chunk)) {
    //     updateChunkStructures(chunk, generationData.blocks);
    // }

    return generationData;
}

void ChunkCreateSystem::updateChunkStructures(Block*** chunkBlocks, BlockCollection structureBlocks) const {
    // for (auto [pos, type] : structureBlocks) {
    //     int x = pos.x, y = pos.y, z = pos.z;
    //     if (chunkBlocks[x][y][z].type == BlockId::BLOCK_AIR) {
    //         chunkBlocks[x][y][z] = Block{type};
    //     }
    // }
}

ChunkGeometryData ChunkCreateSystem::updateChunkVertices(entt::entity entity, const ChunkComponent& chunk) {
    ChunkGeometryData geometryData{entity};

    // reserve some space to prevent reallocations
    try {
        geometryData.verticesCulled.reserve(1048576);
        geometryData.verticesNonCulled.reserve(1048576);
        geometryData.verticesTransparent.reserve(1048576);
        geometryData.indicesCulled.reserve(1048576);
        geometryData.indicesNonCulled.reserve(1048576);
        geometryData.indicesTransparent.reserve(1048576);
    }
    catch (std::length_error e) {
        std::cerr << "WARNING: chunk buffer preallocation failed: " << e.what() << std::endl;
    }

    for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
                bool draw[6] = {false};
                glm::vec3 blockPosition{x, y, z};

                // use shared_lock for shared read access and unique_lock for unique write access
                std::shared_lock<std::shared_mutex> blockLock(*(chunk.blockMutex));
                const Block& block = chunk.getBlock(x, y, z);
                if (block.type == BlockId::BLOCK_AIR) {
                    blockLock.unlock();
                    continue;
                }

                GeometryData blockGeo = BlockGeometry::getGeometry(block);

                // solid blocks
                if (block.type < BlockId::PLANE_GRASS) {
                    if (block.type == BlockId::BLOCK_WATER) {                    
                        if (y < Configuration::CHUNK_HEIGHT - 1 && chunk.getBlock(x, y + 1, z).type != BlockId::BLOCK_WATER) {
                            draw[3] = true;
                        }
                    }
                    else {
                        // negative X
                        if (x > 0) {
                            if (chunk.getBlock(x - 1, y, z).isTransparent()) {
                                draw[0] = true;
                            }
                        }
                        else {
                            draw[0] = true;
                        }
                        // positive X
                        if (x < Configuration::CHUNK_SIZE - 1) {
                            if (chunk.getBlock(x + 1, y, z).isTransparent())
                                draw[1] = true;
                        }
                        else {
                            draw[1] = true;
                        }
                        // negative Y
                        if (y > 0) {
                            if (chunk.getBlock(x, y - 1, z).isTransparent())
                                draw[2] = true;
                        }
                        else {
                            draw[2] = true;
                        }
                        // positive Y
                        if (y < Configuration::CHUNK_HEIGHT - 1) {
                            if (chunk.getBlock(x, y + 1, z).isTransparent())
                                draw[3] = true;
                        }
                        else {
                            draw[3] = true;
                        }

                        // negative Z
                        if (z > 0) {
                            if (chunk.getBlock(x, y, z - 1).isTransparent())
                                draw[4] = true;
                        }
                        else {
                            draw[4] = true;
                        }
                        // positive Z
                        if (z < Configuration::CHUNK_SIZE - 1) {
                            if (chunk.getBlock(x, y, z + 1).isTransparent())
                                draw[5] = true;
                        }
                        else {
                            draw[5] = true;
                        }
                    }

                    blockLock.unlock();

                    const BlockUVs& blockUVs = m_atlas.blockUVsArray[(size_t)block.type];

                    int faceCountPerPass = 0;

                    if (block.isTransparent()) {
                        // add vertices
                        for (size_t face = 0; face < 6; face++) {
                            if (draw[face]) {
                                for (size_t vi = 0; vi < 4; vi++) {
                                    const auto& ref = blockGeo[face * 4 + vi];
                                    geometryData.verticesTransparent.emplace_back(Vertex{glm::vec3{ref[0], ref[1], ref[2]} + blockPosition, glm::vec3{ref[3], ref[4], ref[5]}, glm::vec2{blockUVs[ref[6]][ref[7]]}});
                                }
                                faceCountPerPass++;
                            }
                        }

                        // add indices
                        for (int i = 0; i < faceCountPerPass; i++) {
                            // 6 indices make up a whole face
                            int faceCount = geometryData.indicesTransparent.size() / 6;

                            for (size_t j = 0; j < 6; j++) {
                                constexpr int offsets[] = {0, 1, 2, 0, 3, 1};
                                geometryData.indicesTransparent.emplace_back(faceCount * 4 + offsets[j]);
                            }
                        }
                    }
                    else {
                        // add vertices
                        for (size_t face = 0; face < 6; face++) {
                            if (draw[face]) {
                                for (size_t vi = 0; vi < 4; vi++) {
                                    const auto& ref = blockGeo[face * 4 + vi];
                                    geometryData.verticesCulled.emplace_back(Vertex{glm::vec3{ref[0], ref[1], ref[2]} + blockPosition, glm::vec3{ref[3], ref[4], ref[5]}, glm::vec2{blockUVs[ref[6]][ref[7]]}});
                                }
                                faceCountPerPass++;
                            }
                        }

                        // add indices
                        for (int i = 0; i < faceCountPerPass; i++) {
                            // 6 indices make up a whole face
                            int faceCount = geometryData.indicesCulled.size() / 6;

                            for (size_t j = 0; j < 6; j++) {
                                constexpr int offsets[] = {0, 1, 2, 0, 3, 1};
                                geometryData.indicesCulled.emplace_back(faceCount * 4 + offsets[j]);
                            }
                        }
                    }
                }
                // special blocks using other geometry which are not culled
                else {
                    blockLock.unlock();

                    const FaceUVs& uvs = m_atlas.blockUVsArray[(size_t)block.type][0];

                    for (size_t vi = 0; vi < 8; vi++) {
                        const auto& ref = blockGeo[vi];
                        geometryData.verticesNonCulled.emplace_back(Vertex{glm::vec3{ref[0], ref[1], ref[2]} + blockPosition, glm::vec3{ref[3], ref[4], ref[5]}, uvs[ref[6]]});
                    }

                    for (int i = 0; i < 2; i++) {
                        int faceCount = geometryData.indicesNonCulled.size() / 6;

                        for (size_t j = 0; j < 6; j++) {
                            constexpr int offsets[] = {0, 1, 2, 0, 2, 3};
                            geometryData.indicesNonCulled.emplace_back(faceCount * 4 + offsets[j]);
                        }
                    }
                }
            }
        }
    }

    return geometryData;
}

void ChunkCreateSystem::updateChunkBuffers(Geometry* geometry, const std::vector<unsigned int>& indices,
                                           const std::vector<Vertex>& vertices) {

    geometry->fillBuffers(vertices, indices);
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

            for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
                delete[] chunk.biomes[x];
            }

            delete[] chunk.biomes;

            delete chunk.blockMutex;
            delete chunk.geometryCulled;
            delete chunk.geometryNonCulled;
            delete chunk.geometryTransparent;

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
                        std::launch::async, [=, this]() { return updateChunkBlocks(entity, chunk.chunkX, chunk.chunkZ); }));

                    chunk.verticesOutdated = true;
                }
                // update vertices
                else if (chunk.verticesOutdated) {
                    m_constructionCount++;
                    chunk.threadActiveOnSelf = true;

                    m_geometryFutures.push_back(std::async(std::launch::async, [=, this]() {
                        return updateChunkVertices(entity, chunk);
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

            // chunk.blocks = generationData.blocks;
            chunk.blocks = generationData.blocks;
            chunk.blockData = generationData.blockData;
            chunk.blockStates = generationData.stateData;
            chunk.biomes = generationData.biomes;                        
            // chunk.verticesOutdated = true;
            chunk.threadActiveOnSelf = false;
            m_constructionCount--;

            World::addChunk(generationData.entity, glm::vec2{chunk.chunkX, chunk.chunkZ});

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
            ChunkGeometryData geometryData = itGeo->get();
            ChunkComponent& chunk = m_registry.get<ChunkComponent>(geometryData.entity);

            // culled geometry
            updateChunkBuffers(chunk.geometryCulled, geometryData.indicesCulled, geometryData.verticesCulled);

            // non-culled geometry
            updateChunkBuffers(chunk.geometryNonCulled, geometryData.indicesNonCulled, geometryData.verticesNonCulled);

            // water geometry
            updateChunkBuffers(chunk.geometryTransparent, geometryData.indicesTransparent, geometryData.verticesTransparent);

            chunk.verticesOutdated = false;
            chunk.threadActiveOnSelf = false;
            chunk.needsUpdate = true;
            m_constructionCount--;

            itGeo = m_geometryFutures.erase(itGeo);
        }
        else {
            itGeo++;
        }
    }

    // process structures
    // auto itStr = m_structureQueue.begin();
    // while (itStr != m_structureQueue.end()) {
    //     glm::vec2 chunkPos = (*itStr).first;
    //     if (World::chunkCreated(chunkPos)) {
    //         ChunkComponent& chunk = m_registry.get<ChunkComponent>(World::getChunk(chunkPos));
    //         if (chunk.blocks) {
    //             updateChunkStructures(chunk.blocks, (*itStr).second);

    //             chunk.verticesOutdated = true;
    //             itStr = m_structureQueue.erase(itStr);
    //         }
    //         else {
    //             itStr++;
    //         }
    //     }
    //     else {
    //         itStr++;
    //     }
    // }
}

ChunkCreateSystem::ChunkCreateSystem(Registry_T& registry, const TextureAtlas& atlas)
    : System{registry, 10}, m_worldGenerator{WorldType::WORLD_NORMAL},
      m_structureGenerator{&m_worldGenerator}, m_atlas{atlas} {

    // event callbacks
    m_playerMovedHandle = EventDispatcher::onEntityMoved.subscribe(
        [&](const EntityMovedEvent& e) {
            if (m_registry.any<PlayerComponent>(e.entity)) {
                handlePlayerMoved(e);
            }
        });

    m_structureCreatedHandle = EventDispatcher::onStructureCreated.subscribe(
        [&](const StructureCreatedEvent& e) { handleStructureCreated(e); });

    handlePlayerMoved(EntityMovedEvent{nullptr, entt::null, glm::vec3{0.f}, glm::vec3{FLT_MAX}});
}

int ChunkCreateSystem::getActiveChunkCount() const {
    return m_loadedChunks.size();
}
