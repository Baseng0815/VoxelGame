#include "System.h"
#include "WorldGenerator.h"

struct ChunkHashFunction {
    size_t operator()(const glm::vec2& v) const {
        return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
    }

    bool operator()(const glm::vec2& l, const glm::vec2& r) const {
        return l.x == r.x && l.y == r.y;
    }
};

class ChunkCreateSystem {
    private:
        WorldGenerator m_generator;

        void handleChunkEnter(int x, int z);

        void updateChunkBlocks(ChunkComponent& chunk);
        void updateChunkVertices(ChunkComponent& chunk, GeometryComponent& geometryComponent);
        void updateChunkBuffers(ChunkComponent& chunk, GeometryComponent& geometryComponent);

    public:
        ChunkCreateSystem(SystemManager* systemManager, WorldType worldType);

        void update(int dt) override;
        void handleEvent(Event* e) override;

        ~ChunkUpdateSystem() override;
}
