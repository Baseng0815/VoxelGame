#pragma once

#include "System.h"
#include "../../include/WorldGeneration/WorldGenerator.h"

#include <map>
#include <mutex>
#include <atomic>
#include <future>
#include <entt/entt.hpp>

struct Event;
class ChunkComponent;
class GeometryComponent;
class AtlasComponent;
class Vertex;

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

        std::atomic_int constructionCount;
        std::vector<entt::entity> m_destructionQueue;
        std::vector<glm::vec2> loadedChunks;

        void handleEnterChunk(Event*);
        void handleBlockChanged(Event*);

        void updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ);
        void updateChunkVertices(entt::entity entity, Block*** blocks,
                const AtlasComponent& atlas, std::mutex* blockMutex);
        void updateChunkBuffers(GeometryComponent& geometryComponent,
                const std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices);

        void _update(int dt) override;

    public:
        ChunkCreateSystem(entt::registry* registry);
};
