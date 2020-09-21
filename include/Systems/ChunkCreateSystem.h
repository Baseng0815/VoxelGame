#pragma once

#include "System.h"

#include "../Utility.h"
#include "../Resources/Geometry.h"
#include "../WorldGeneration/WorldGenerator.h"
#include "../WorldGeneration/StructureGenerator.h"

#include <queue>
#include <unordered_map>
#include <atomic>
#include <future>
#include <shared_mutex>
#include <entt/entt.hpp>

struct EnterChunkEvent;
struct BlockChangedEvent;
struct StructureCreatedEvent;

struct Vertex;
struct AtlasComponent;
struct ChunkComponent;

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

        WorldGenerator m_worldGenerator = WorldGenerator{WorldType::WORLD_NORMAL};
        StructureGenerator m_structureGenerator;
        std::unordered_map<glm::vec2, BlockCollection, Utility::HashFunctionVec2> m_structureQueue;

        int m_constructionCount;
        std::vector<entt::entity> m_destructionQueue;
        std::vector<glm::vec2> m_loadedChunks;        

        CallbackHandle<const EnterChunkEvent&> m_enterChunkHandle;
        void handleEnterChunk(const EnterChunkEvent& e);
        CallbackHandle<const BlockChangedEvent&> m_blockChangeHandle;
        void handleBlockChanged(const BlockChangedEvent& e);
        CallbackHandle<const StructureCreatedEvent&> m_structureCreatedHandle;        
        void handleStructureCreated(const StructureCreatedEvent& e);

        GenerationData updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ);
        void updateChunkStructures(glm::vec2 chunkPos, Block*** blocks);
        GeometryData updateChunkVertices(entt::entity entity, Block ***blocks, std::shared_mutex *blockMutex, const AtlasComponent &atlas);
        void updateChunkBuffers(Geometry& geometryComponent, const std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices);        


        void _update(int dt) override;

    public:
        ChunkCreateSystem(Registry_T &registry);

        int getActiveChunkCount() const;
};
