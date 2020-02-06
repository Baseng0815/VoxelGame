#pragma once

#include "System.h"
#include "Components.h"
#include "WorldGenerator.h"

struct ChunkHashFunction {
    size_t operator()(const glm::vec2& v) const {
        return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
    }

    bool operator()(const glm::vec2& l, const glm::vec2& r) const {
        return l.x == r.x && l.y == r.y;
    }
};

class EnterChunkEvent;

class ChunkCreateSystem : public System {
    private:
        WorldGenerator m_generator;

        std::atomic_int constructionCount;
        std::vector<glm::vec2> loadedChunks;
        BlockUVsArray blockUVsArray;

        void handleEnterChunk(EnterChunkEvent*);

        static void updateChunkBlocks(ChunkComponent& chunk);
        static void updateChunkVertices(ChunkComponent& chunk, GeometryComponent& geometryComponent);
        static void updateChunkBuffers(ChunkComponent& chunk, GeometryComponent& geometryComponent);

    public:
        ChunkCreateSystem(SystemManager* systemManager, WorldType worldType);

        void update(int dt) override;
        void handleEvent(Event* e) override;

        ~ChunkCreateSystem();
};
