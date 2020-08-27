#pragma once

#include "System.h"
#include "../WorldGeneration/WorldGenerator.h"
#include "../Resources/Geometry.h"

#include <map>
#include <mutex>
#include <atomic>
#include <future>
#include <entt/entt.hpp>

struct EnterChunkEvent;
struct BlockChangedEvent;

class Vertex;
class AtlasComponent;
class ChunkComponent;

class ChunkCreateSystem : public System {
    private:
        struct GeometryData {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
        };

        struct GenerationData {
            Block*** blocks;
            BiomeID** biomes;
        };

        std::mutex m_blockMapMutex, m_geometryMapMutex;

        std::vector<entt::entity> m_outdatedChunks;
        std::map<entt::entity, GenerationData> m_finishedBlocks;
        std::map<entt::entity, GeometryData> m_finishedGeometries;
        std::vector<std::future<void>> m_futures;

        WorldGenerator m_generator = WorldGenerator(WorldType::WORLD_NORMAL);

        std::atomic_int m_constructionCount;
        std::vector<entt::entity> m_destructionQueue;
        std::vector<glm::vec2> m_loadedChunks;

        CallbackHandle<const EnterChunkEvent&> m_enterChunkHandle;
        void handleEnterChunk(const EnterChunkEvent& e);
        CallbackHandle<const BlockChangedEvent&> m_blockChangeHandle;
        void handleBlockChanged(const BlockChangedEvent& e);

        void updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ);
        void updateChunkVertices(entt::entity entity, Block*** blocks,
                const AtlasComponent& atlas, std::mutex* blockMutex);
        void updateChunkBuffers(Geometry& geometryComponent,
                const std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices);

        void _update(int dt) override;

    public:
        ChunkCreateSystem(entt::registry* registry);

        int getActiveChunkCount() const;
};
