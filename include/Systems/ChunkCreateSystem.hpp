#pragma once

#include "System.hpp"

#include "../Resources/Geometry.hpp"
#include "../Utility.hpp"
#include "../WorldGeneration/GenerationData.hpp"
#include "../WorldGeneration/StructureGenerator.hpp"
#include "../WorldGeneration/WorldGenerator.hpp"

#include "../GameData/BiomeIds.hpp"
#include "../GameData/BlockIds.hpp"

#include <atomic>
#include <entt/entt.hpp>
#include <future>
#include <glm/gtx/hash.hpp>
#include <map>
#include <shared_mutex>

struct EntityMovedEvent;
struct BlockChangedEvent;

struct Vertex;
struct ChunkComponent;
struct TextureAtlas;
struct BlockState;

struct ChunkGeometryData {
    entt::entity entity;
    std::vector<Vertex> verticesCulled;
    std::vector<Vertex> verticesNonCulled;
    std::vector<Vertex> verticesTransparent;
    std::vector<unsigned int> indicesCulled;
    std::vector<unsigned int> indicesNonCulled;
    std::vector<unsigned int> indicesTransparent;
};

class ChunkCreateSystem : public System {
  private:
    std::vector<std::future<GenerationData>> m_generationFutures;
    std::vector<std::future<ChunkGeometryData>> m_geometryFutures;

    // some static data needed

    WorldGenerator m_worldGenerator;
    StructureGenerator m_structureGenerator;    
    const TextureAtlas& m_atlas;

    int m_constructionCount = 0;
    std::vector<entt::entity> m_destructionQueue;
    std::vector<glm::vec2> m_loadedChunks;

    CallbackHandle<const EntityMovedEvent&> m_playerMovedHandle;
    void handlePlayerMoved(const EntityMovedEvent& e);    

    GenerationData updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ);    
    ChunkGeometryData updateChunkVertices(entt::entity entity, const ChunkComponent& chunk);
    void updateChunkBuffers(Geometry* geometryComponent, const std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices);

    void _update(int dt) override;

  public:
    // TODO make atlas a resource or entity
    ChunkCreateSystem(Registry_T& registry, const TextureAtlas& atlas);

    int getActiveChunkCount() const;
};
