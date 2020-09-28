#include "../../include/Systems/ChunkCreateSystem.hpp"

#include "../../include/World.hpp"
#include "../../include/Math/Cuboid.hpp"
#include "../../include/Configuration.hpp"
#include "../../include/GameData/GameData.hpp"
#include "../../include/Events/EventDispatcher.hpp"

#include "../../include/Resources/Texture.hpp"
#include "../../include/Resources/ResourceManager.hpp"

#include "../../include/Components/ChunkComponent.hpp"
#include "../../include/Components/MeshRenderComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"

// this could probably be way better, but w/e
// TODO put in file
const std::array<std::array<float, 8>, 24> ChunkCreateSystem::generationDataBlock = {
    //                    x  y  z  nx  ny nz ix iy
    std::array<float, 8> {0, 1, 0, -1, 0, 0, 4, 0},
    std::array<float, 8> {0, 0, 1, -1, 0, 0, 4, 1},
    std::array<float, 8> {0, 1, 1, -1, 0, 0, 4, 2},
    std::array<float, 8> {0, 0, 0, -1, 0, 0, 4, 3},
    std::array<float, 8> {1, 1, 1, 1, 0, 0, 2, 0},
    std::array<float, 8> {1, 0, 0, 1, 0, 0, 2, 1},
    std::array<float, 8> {1, 1, 0, 1, 0, 0, 2, 2},
    std::array<float, 8> {1, 0, 1, 1, 0, 0, 2, 3},
    std::array<float, 8> {0, 0, 1, 0, -1, 0, 5, 0},
    std::array<float, 8> {1, 0, 0, 0, -1, 0, 5, 1},
    std::array<float, 8> {1, 0, 1, 0, -1, 0, 5, 2},
    std::array<float, 8> {0, 0, 0, 0, -1, 0, 5, 3},
    std::array<float, 8> {0, 1, 0, 0, 1, 0, 0, 0},
    std::array<float, 8> {1, 1, 1, 0, 1, 0, 0, 1},
    std::array<float, 8> {1, 1, 0, 0, 1, 0, 0, 2},
    std::array<float, 8> {0, 1, 1, 0, 1, 0, 0, 3},
    std::array<float, 8> {1, 1, 0, 0, 0, -1, 3, 0},
    std::array<float, 8> {0, 0, 0, 0, 0, -1, 3, 1},
    std::array<float, 8> {0, 1, 0, 0, 0, -1, 3, 2},
    std::array<float, 8> {1, 0, 0, 0, 0, -1, 3, 3},
    std::array<float, 8> {0, 1, 1, 0, 0, 1, 1, 0},
    std::array<float, 8> {1, 0, 1, 0, 0, 1, 1, 1},
    std::array<float, 8> {1, 1, 1, 0, 0, 1, 1, 2},
    std::array<float, 8> {0, 0, 1, 0, 0, 1, 1, 3}
};

const std::array<std::array<float, 8>, 24> ChunkCreateSystem::generationDataPlane = {
    //                    x  y  z  nx  ny nz fi
    std::array<float, 8> {0, 0, 0, 1, 0, -1, 0},
    std::array<float, 8> {0, 1, 0, 1, 0, -1, 1},
    std::array<float, 8> {1, 1, 1, 1, 0, -1, 2},
    std::array<float, 8> {1, 0, 1, 1, 0, -1, 3},
    std::array<float, 8> {1, 0, 0, -1, 0, 1, 0},
    std::array<float, 8> {1, 1, 0, -1, 0, 1, 1},
    std::array<float, 8> {0, 1, 1, -1, 0, 1, 2},
    std::array<float, 8> {0, 0, 1, -1, 0, 1, 3},
};

void ChunkCreateSystem::handleEnterChunk(const EnterChunkEvent &e)
{
    // queue chunk deletion
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
    for (int x = e.newX + Configuration::CHUNK_PRELOAD_SIZE; x >= e.newX - (int)Configuration::CHUNK_PRELOAD_SIZE; x--) {
        for (int z = e.newZ + Configuration::CHUNK_PRELOAD_SIZE; z >= e.newZ - (int)Configuration::CHUNK_PRELOAD_SIZE; z--) {
            glm::vec2 pos{x, z};
            if (std::count(m_loadedChunks.begin(), m_loadedChunks.end(), pos) == 0) {
                entt::entity entity = m_registry.create();

                const ChunkComponent &chunk = m_registry.emplace<ChunkComponent>(entity, new std::shared_mutex{}, x, z, new Geometry {});
                m_registry.emplace<TransformationComponent>(entity, glm::vec3 {x * Configuration::CHUNK_SIZE, 0, z * Configuration::CHUNK_SIZE});
                // TODO add two mesh renderers; one for solid blocks with culling and one for planes without culling
                m_registry.emplace<MeshRenderComponent>(entity, ResourceManager::getResource<Material>(MATERIAL_CHUNK_BLOCKS), chunk.geometry);

                m_loadedChunks.push_back(pos);
            }
        }
    }
}

void ChunkCreateSystem::handleStructureCreated(const StructureCreatedEvent& e) {
    for (const auto &[chunk, blocks] : e.data) {
        if (m_structureQueue.contains(chunk)) {
            for (const auto &block : blocks) {
                m_structureQueue[chunk].emplace_back(block);
            }
        }
        else {
            m_structureQueue.emplace(chunk, blocks);
        }

        if (World::chunkCreated(chunk)) {
            ChunkComponent& chunkComponent = m_registry.get<ChunkComponent>(World::getChunk(chunk));
            chunkComponent.structuresOutdated = true;
        }
    }
}

GenerationData ChunkCreateSystem::updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ) {
    GenerationData generationData;
    generationData.entity = entity;

    // allocate blocks
    generationData.blocks = new Block**[Configuration::CHUNK_SIZE];
    for (int x = 0; x < Configuration::CHUNK_SIZE; x++) {
        generationData.blocks[x] = new Block*[Configuration::CHUNK_HEIGHT];
        for (int y = 0; y < Configuration::CHUNK_HEIGHT; y++) {
            generationData.blocks[x][y] = new Block[Configuration::CHUNK_SIZE] { Block {BlockId::BLOCK_AIR} };

            if (y < 63) {
                for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
                    generationData.blocks[x][y][z] = Block {BlockId::BLOCK_WATER};
                }
            }
        }
    }

    // allocate biomes
    generationData.biomes = new BiomeId*[Configuration::CHUNK_SIZE];
    for (int z = 0; z < Configuration::CHUNK_SIZE; z++) {
        generationData.biomes[z] = new BiomeId[Configuration::CHUNK_SIZE];
    }

    glm::vec2 chunk = glm::vec2{chunkX, chunkZ};
    m_worldGenerator.generate(chunk, &generationData);
    m_structureGenerator.generateStructures(chunk, &generationData);

    if (m_structureQueue.contains(chunk)) {
        updateChunkStructures(chunk, generationData.blocks);
    }

    return generationData;
}

void ChunkCreateSystem::updateChunkStructures(glm::vec2 chunkPosition, Block*** blocks) {
    BlockCollection structureBlocks = m_structureQueue[chunkPosition];
    for (auto [pos, type] : structureBlocks) {
        int x = pos.x, y = pos.y, z = pos.z;
        if (blocks[x][y][z].type == BlockId::BLOCK_AIR) {
            blocks[x][y][z] = Block {type};
        }
    }

    m_structureQueue.erase(chunkPosition);
}

GeometryData ChunkCreateSystem::updateChunkVertices(entt::entity entity, Block*** blocks, std::shared_mutex* blockMutex) {
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
                glm::vec3 blockPosition {x, y, z};

                // use shared_lock for shared read access and unique_lock for unique write access
                std::shared_lock<std::shared_mutex> blockLock(*blockMutex);
                const Block &block = blocks[x][y][z];
                if (block.type == BlockId::BLOCK_AIR) {
                    continue;
                }

                // water
                if (block.type == BlockId::BLOCK_WATER) {
                    if (y < Configuration::CHUNK_HEIGHT - 1 && blocks[x][y + 1][z].type == BlockId::BLOCK_AIR) {
                        draw[3] = true;
                    }
                }

                // solid blocks
                else if (block.type < BlockId::PLANE_GRASS) {
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

                    blockLock.unlock();

                    const BlockUVs &blockUVs = m_atlas.blockUVsArray[(size_t)block.type];

                    int faceCountPerPass = 0;

                    // add vertices
                    for (size_t face = 0; face < 6; face++) {
                        if (draw[face]) {
                            for (size_t vi = 0; vi < 4; vi++) {
                                const auto &ref = generationDataBlock[face * 4 + vi];
                                geometryData.vertices.emplace_back(Vertex {glm::vec3 {ref[0], ref[1], ref[2]} + blockPosition, glm::vec3 {ref[3], ref[4], ref[5]}, glm::vec2 {blockUVs[ref[6]][ref[7]]}});
                            }
                            faceCountPerPass++;
                        }
                    }

                    // add indices
                    for (int i = 0; i < faceCountPerPass; i++) {

                        for (size_t j = 0; j < 6; j++) {
                            constexpr int offsets[] = {0, 1, 2, 0, 3, 1};
                            geometryData.indices.emplace_back(faceCount * 4 + offsets[j]);
                        }
                        faceCount++;
                    }
                }
                // special blocks using 2d planes
                else {
                    const FaceUVs &uvs = m_atlas.blockUVsArray[(size_t)block.type][0];

                    for (size_t vi = 0; vi < 8; vi++) {
                        const auto &ref = generationDataPlane[vi];
                        geometryData.vertices.emplace_back(Vertex {glm::vec3 {ref[0], ref[1], ref[2]} + blockPosition, glm::vec3 {ref[3], ref[4], ref[5]}, uvs[ref[6]]});
                    }

                    for (int i = 0; i < 2; i++) {
                        for (size_t j = 0; j < 6; j++) {
                            constexpr int offsets[] = {0, 1, 2, 0, 2, 3};
                            geometryData.indices.emplace_back(faceCount * 4 + offsets[j]);
                        }
                        // yes, faceCount could be calculated using indices.size()
                        faceCount++;
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
                else if (chunk.structuresOutdated) {
                    updateChunkStructures(glm::vec2{chunk.chunkX, chunk.chunkZ}, chunk.blocks);

                    chunk.structuresOutdated = false;
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
            ChunkComponent& chunk = m_registry.get<ChunkComponent>(generationData.entity);

            chunk.blocks = generationData.blocks;
            for (int x = 0; x < 16; x++) {
                chunk.blocks[x][80][0] = Block {BlockId::PLANE_GRASS};
            }
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
}

ChunkCreateSystem::ChunkCreateSystem(Registry_T& registry)
    : System {registry, 10}, m_worldGenerator {WorldType::WORLD_NORMAL}, m_structureGenerator {&m_worldGenerator}
{
    const Texture *atlasTexture = ResourceManager::getResource<Texture>(TEXTURE_ATLAS);
    int width = atlasTexture->getWidth();
    int height = atlasTexture->getHeight();

    // atlas
    m_atlas.numCols = width / 16.f;
    m_atlas.numRows = height / 16.f;
    m_atlas.uvXpT = 1 / (float)m_atlas.numCols;
    m_atlas.uvYpT = 1 / (float)m_atlas.numRows;

    // generate face uvs
    for (int i = 0; i < m_atlas.numCols * m_atlas.numRows; i++) {
        glm::vec2 topLeft = glm::vec2(i % m_atlas.numCols / (float)m_atlas.numCols , i / m_atlas.numCols / (float)m_atlas.numRows);
        m_atlas.faceUVsArray.push_back({topLeft,
            glm::vec2 {topLeft.x + m_atlas.uvXpT, topLeft.y + m_atlas.uvYpT},
            glm::vec2 {topLeft.x + m_atlas.uvXpT, topLeft.y},
            glm::vec2 {topLeft.x, topLeft.y + m_atlas.uvYpT}});
    }

    // generate block uv array
    for (size_t i = 0; i < m_atlas.blockUVsArray.size(); i++) {
        const BlockTemplate &blockTemplate = GameData::getBlockTemplate((BlockId)i);
        m_atlas.blockUVsArray[i][0] = m_atlas.faceUVsArray[blockTemplate.tid_py];
        m_atlas.blockUVsArray[i][1] = m_atlas.faceUVsArray[blockTemplate.tid_nx];
        m_atlas.blockUVsArray[i][2] = m_atlas.faceUVsArray[blockTemplate.tid_px];
        m_atlas.blockUVsArray[i][3] = m_atlas.faceUVsArray[blockTemplate.tid_nz];
        m_atlas.blockUVsArray[i][4] = m_atlas.faceUVsArray[blockTemplate.tid_pz];
        m_atlas.blockUVsArray[i][5] = m_atlas.faceUVsArray[blockTemplate.tid_ny];
    }

    // event callbacks
    m_enterChunkHandle =
        EventDispatcher::onEnterChunk.subscribe([&](const EnterChunkEvent& e) { handleEnterChunk(e); });

    m_structureCreatedHandle = EventDispatcher::onStructureCreated.subscribe(
        [&](const StructureCreatedEvent& e) { handleStructureCreated(e); });

    handleEnterChunk(EnterChunkEvent());

}

int ChunkCreateSystem::getActiveChunkCount() const {
    return m_loadedChunks.size();
}
