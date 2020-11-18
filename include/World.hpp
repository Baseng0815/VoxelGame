#include "../include/Utility.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

struct Block;
struct ChunkComponent;

class World {
    private:
        static std::unordered_map<glm::ivec2, entt::entity, std::hash<glm::ivec2>> chunksLookup;
        static std::unordered_map<glm::ivec3, entt::entity, std::hash<glm::ivec3>> blockStates;

      public:
        static ChunkComponent& getChunk(entt::registry& registry, const glm::vec2& chunk);
        static bool chunkCreated(const glm::vec2 &chunk);

        static Block getBlock(entt::registry& registry, const glm::vec3 &position);
        static void setBlock(entt::registry& registry, Block block);

        static void addChunk(entt::entity entity, const glm::vec2 &position);
        static void removeChunk(entt::entity entity);
};
