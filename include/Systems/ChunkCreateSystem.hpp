#pragma once

#include "System.hpp"

#include "../Utility.hpp"
#include "../Resources/Geometry.hpp"
#include "../WorldGeneration/WorldGenerator.hpp"
#include "../WorldGeneration/StructureGenerator.hpp"

#include "../GameData/BiomeIds.hpp"
#include "../GameData/BlockIds.hpp"

#include <atomic>
#include <entt/entt.hpp>
#include <future>
#include <map>
#include <shared_mutex>

struct EntityMovedEvent;
struct BlockChangedEvent;
struct StructureCreatedEvent;

struct Vertex;
struct ChunkComponent;
struct TextureAtlas;

struct GenerationData {
    entt::entity entity;
    Block ***blocks;
    BiomeId **biomes;
};

struct GeometryData {
    entt::entity entity;
    std::vector<Vertex> verticesCulled;
    std::vector<Vertex> verticesNonCulled;
    std::vector<Vertex> verticesWater;
    std::vector<unsigned int> indicesCulled;
    std::vector<unsigned int> indicesNonCulled;
    std::vector<unsigned int> indicesWater;
};

class ChunkCreateSystem : public System {
    private:
        std::vector<std::future<GenerationData>> m_generationFutures;
        std::vector<std::future<GeometryData>> m_geometryFutures;

        // some static data needed
        static const std::array<std::array<float, 8>, 24> generationDataBlock;
        static const std::array<std::array<float, 8>, 8> generationDataPlane;

        WorldGenerator m_worldGenerator;
        StructureGenerator m_structureGenerator;
        std::unordered_map<glm::vec2, BlockCollection, Utility::HashFunctionVec2> m_structureQueue;
        const TextureAtlas &m_atlas;

        int m_constructionCount = 0;
        std::vector<entt::entity> m_destructionQueue;
        std::vector<glm::vec2> m_loadedChunks;

        CallbackHandle<const EntityMovedEvent &> m_playerMovedHandle;
        void handlePlayerMoved(const EntityMovedEvent &e);
        CallbackHandle<const StructureCreatedEvent &> m_structureCreatedHandle;
        void handleStructureCreated(const StructureCreatedEvent &e);

        GenerationData updateChunkBlocks(entt::entity entity, int chunkX, int chunkZ);
        void updateChunkStructures(Block ***chunkBlocks, BlockCollection structureBlocks) const;
        GeometryData updateChunkVertices(entt::entity entity, Block ***blocks, std::shared_mutex *blockMutex);
        void updateChunkBuffers(Geometry *geometryComponent, const std::vector<unsigned int> &indices, const std::vector<Vertex> &vertices);

        void _update(int dt) override;

    public:
        // TODO make atlas a resource or entity
        ChunkCreateSystem(Registry_T &registry, const TextureAtlas &atlas);

        int getActiveChunkCount() const;
};
