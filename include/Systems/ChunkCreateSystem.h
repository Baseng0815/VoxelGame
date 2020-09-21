#pragma once

#include "System.h"

#include "../Utility.h"
#include "../Resources/Geometry.h"
#include "../WorldGeneration/WorldGenerator.h"
#include "../WorldGeneration/StructureGenerator.h"

#include "../GameData/BlockIds.h"
#include "../GameData/BiomeIds.h"

#include <map>
#include <atomic>
#include <future>
#include <shared_mutex>
#include <entt/entt.hpp>

struct EnterChunkEvent;
struct BlockChangedEvent;
struct StructureCreatedEvent;

struct Vertex;
struct ChunkComponent;

struct GenerationData {
    entt::entity entity;
    Block*** blocks;
    BiomeId** biomes;
};

struct GeometryData {
    entt::entity entity;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

using FaceUVs = std::array<glm::vec2, 4>;
using BlockUVs = std::array<FaceUVs, 6>;
using BlockUVsArray = std::array<BlockUVs, (size_t)BlockId::NUM_BLOCKS>;

struct Atlas {
    int numRows, numCols;
    float uvXpT, uvYpT;

    BlockUVsArray blockUVsArray;
};

class ChunkCreateSystem : public System {
    private:
        std::vector<std::future<GenerationData>> m_generationFutures;
        std::vector<std::future<GeometryData>> m_geometryFutures;

        WorldGenerator m_worldGenerator;
        StructureGenerator m_structureGenerator;
        std::unordered_map<glm::vec2, BlockCollection, Utility::HashFunctionVec2> m_structureQueue;
        Atlas m_atlas;

        int m_constructionCount = 0;
        std::vector<entt::entity> m_destructionQueue;
        std::vector<glm::vec2> m_loadedChunks;

        CallbackHandle<const EnterChunkEvent&> m_enterChunkHandle;
        void handleEnterChunk(const EnterChunkEvent& e);
        CallbackHandle<const StructureCreatedEvent&> m_structureCreatedHandle;        
        void handleStructureCreated(const StructureCreatedEvent& e);

        GenerationData updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ);
        void updateChunkStructures(glm::vec2 chunkPos, Block*** blocks);
        GeometryData updateChunkVertices(entt::entity entity, Block ***blocks, std::shared_mutex *blockMutex);
        void updateChunkBuffers(Geometry& geometryComponent, const std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices);        


        void _update(int dt) override;

    public:
        ChunkCreateSystem(Registry_T &registry);

        int getActiveChunkCount() const;
};
