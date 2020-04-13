#pragma once

#include "System.h"
#include "WorldGenerator.h"
#include "TextureAtlas.h"

struct ChunkHashFunction {
    size_t operator()(const glm::vec2& v) const {
        return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
    }

    bool operator()(const glm::vec2& l, const glm::vec2& r) const {
        return l.x == r.x && l.y == r.y;
    }
};

class Event;
class ChunkComponent;
class GeometryComponent;

class ChunkCreateSystem : public System {
    private:
        WorldGenerator m_generator;

        std::atomic_int constructionCount;
        std::vector<glm::vec2> loadedChunks;

        // TODO introduce enter chunk event and chunk generation
        void handleEnterChunk(Event*);

        void updateChunkBlocks(ChunkComponent& chunk);
        void updateChunkVertices(ChunkComponent& chunk, GeometryComponent& geometryComponent);
        void updateChunkBuffers(ChunkComponent& chunk, GeometryComponent& geometryComponent);

    public:
        ChunkCreateSystem(SystemManager* systemManager, SharedContext *context, WorldType worldType);

        void init() override;
        void update(int dt) override;

        ~ChunkCreateSystem();
};
