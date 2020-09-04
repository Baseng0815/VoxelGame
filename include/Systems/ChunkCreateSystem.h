#pragma once

#include "System.h"
#include "../WorldGeneration/WorldGenerator.h"
#include "../Resources/Geometry.h"

#include <map>
#include <atomic>
#include <shared_mutex>
#include <future>
#include <entt/entt.hpp>

struct EnterChunkEvent;
struct BlockChangedEvent;

class Vertex;
class AtlasComponent;
class ChunkComponent;

struct GenerationData {
    entt::entity entity;
    Block*** blocks;
    BiomeID** biomes;
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

        WorldGenerator m_generator = WorldGenerator(WorldType::WORLD_NORMAL);

        std::atomic_int m_constructionCount;
        std::vector<entt::entity> m_destructionQueue;
        std::vector<glm::vec2> m_loadedChunks;        

        CallbackHandle<const EnterChunkEvent&> m_enterChunkHandle;
        void handleEnterChunk(const EnterChunkEvent& e);
        CallbackHandle<const BlockChangedEvent&> m_blockChangeHandle;
        void handleBlockChanged(const BlockChangedEvent& e);

        GenerationData updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ);
        GeometryData updateChunkVertices(entt::entity entity, Block ***blocks, std::shared_mutex *blockMutex, const AtlasComponent &atlas);
        void updateChunkBuffers(Geometry& geometryComponent,
                const std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices);        

        void _update(int dt) override;

    public:
        ChunkCreateSystem(Registry_T &registry);

        int getActiveChunkCount() const;
};
