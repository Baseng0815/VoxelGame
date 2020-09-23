#pragma once

#include "System.hpp"

#include "../Resources/Geometry.hpp"
#include "../Resources/TextureAtlas.hpp"
#include "../Utility.hpp"
#include "../WorldGeneration/StructureGenerator.hpp"
#include "../WorldGeneration/WorldGenerator.hpp"

#include "../GameData/BiomeIds.hpp"
#include "../GameData/BlockIds.hpp"

#include <atomic>
#include <entt/entt.hpp>
#include <future>
#include <map>
#include <shared_mutex>

struct EnterChunkEvent;
struct BlockChangedEvent;
struct StructureCreatedEvent;

struct Vertex;
struct ChunkComponent;

struct GenerationData {
    entt::entity entity;
    Block ***blocks;
    BiomeId **biomes;
};

struct GeometryData {
    entt::entity entity;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

class ChunkCreateSystem : public System {
  private:
    std::vector<std::future<GenerationData>> m_generationFutures;
    std::vector<std::future<GeometryData>> m_geometryFutures;

    WorldGenerator m_worldGenerator;
    StructureGenerator m_structureGenerator;
    std::unordered_map<glm::vec2, BlockCollection, Utility::HashFunctionVec2> m_structureQueue;
    const TextureAtlas &m_atlas;

    int m_constructionCount = 0;
    std::vector<entt::entity> m_destructionQueue;
    std::vector<glm::vec2> m_loadedChunks;

    CallbackHandle<const EnterChunkEvent &> m_enterChunkHandle;
    void handleEnterChunk(const EnterChunkEvent &e);
    CallbackHandle<const StructureCreatedEvent &> m_structureCreatedHandle;
    void handleStructureCreated(const StructureCreatedEvent &e);

    GenerationData updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ);
    void updateChunkStructures(glm::vec2 chunkPos, Block ***blocks);
    GeometryData updateChunkVertices(entt::entity entity, Block ***blocks, std::shared_mutex *blockMutex);
    void updateChunkBuffers(Geometry *geometryComponent, const std::vector<unsigned int> &indices, const std::vector<Vertex> &vertices);

    void _update(int dt) override;

  public:
    ChunkCreateSystem(Registry_T &registry, const TextureAtlas &atlas);

    int getActiveChunkCount() const;
};
