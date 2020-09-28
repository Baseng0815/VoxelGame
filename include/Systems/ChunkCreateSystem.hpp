#pragma once

#include "System.hpp"

#include "../Utility.hpp"
#include "../Resources/Geometry.hpp"
#include "../WorldGeneration/WorldGenerator.hpp"
#include "../WorldGeneration/StructureGenerator.hpp"

#include "../GameData/BlockIds.hpp"
#include "../GameData/BiomeIds.hpp"

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

#ifndef __ATLAS__
#define __ATLAS__

using FaceUVs = std::array<glm::vec2, 4>;
using BlockUVs = std::array<FaceUVs, 6>;
using FaceUVsArray = std::vector<FaceUVs>;
using BlockUVsArray = std::array<BlockUVs, (size_t)BlockId::NUM_BLOCKS>;

struct Atlas {
    int numRows, numCols;
    float uvXpT, uvYpT;

    FaceUVsArray faceUVsArray;
    BlockUVsArray blockUVsArray;
};
#endif

class ChunkCreateSystem : public System {
    private:
        std::vector<std::future<GenerationData>> m_generationFutures;
        std::vector<std::future<GeometryData>> m_geometryFutures;

        // some static data needed
        static const std::array<std::array<float, 8>, 24> generationDataBlock;
        static const std::array<std::array<float, 8>, 24> generationDataPlane;

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
        void updateChunkBuffers(Geometry *geometryComponent, const std::vector<unsigned int> &indices, const std::vector<Vertex> &vertices);

        void _update(int dt) override;

    public:
        ChunkCreateSystem(Registry_T &registry);

        int getActiveChunkCount() const;
};
