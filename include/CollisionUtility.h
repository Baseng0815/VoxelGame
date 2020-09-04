#include <vector>

struct Cuboid;
struct ChunkComponent;

class CollisionUtility {
    static std::vector<Cuboid> getChunkCollisions(const ChunkComponent& chunk);

    static std::vector<Cuboid> getCollisions(const Block*** blocks, glm::ivec3 start, int size);
};