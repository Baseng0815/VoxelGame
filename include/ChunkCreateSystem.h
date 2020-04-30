#pragma once

#include "System.h"
#include "WorldGenerator.h"
#include "TextureAtlas.h"

#include <map>
#include <mutex>
#include <atomic>
#include <future>
#include <entt/entt.hpp>

class Event;
class ChunkComponent;
class GeometryComponent;
class Vertex;

class ChunkCreateSystem : public System {
    private:
        struct GeometryData {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
        };

        std::mutex m_blockMapMutex, m_geometryMapMutex;

        std::map<entt::entity, Block***> m_finishedBlocks;
        std::map<entt::entity, GeometryData> m_finishedGeometries;
        std::vector<std::future<void>> m_futures;

        WorldGenerator m_generator;

        std::atomic_int constructionCount;
        std::vector<entt::entity> m_destructionQueue;
        std::vector<glm::vec2> loadedChunks;

        void handleEnterChunk(Event*);

        void updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ);
        void updateChunkVertices(entt::entity entity, Block*** blocks, std::mutex* blockMutex);
        void updateChunkBuffers(GeometryComponent& geometryComponent,
                const std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices);

    public:
        ChunkCreateSystem(SystemManager* systemManager);

        void update(int dt) override;
};
