#include "../include/Utility.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

struct Block;
struct BlockState;
struct BlockStateComponent;

class World {
    private:
        static std::unordered_map<glm::ivec2, entt::entity, std::hash<glm::ivec2>> chunksLookup;
        static std::unordered_map<glm::ivec3, entt::entity, std::hash<glm::ivec3>> blockStates;

      public:
        static entt::entity getChunk(const glm::vec2 &chunk);
        static bool chunkCreated(const glm::vec2 &chunk);

        static Block getBlock(const entt::registry *registry, const glm::vec3 &position);
        static void setBlock(entt::registry *registry, const glm::vec3 &position, Block block);

        static BlockStateComponent& getBlockState(entt::registry& registry, const glm::vec3& blockPos);
        static void setBlockState(entt::registry& registry, const glm::vec3& blockPos, BlockState& state);

        static void addChunk(entt::entity entity, const glm::vec2 &position);
        static void removeChunk(entt::entity entity);
};
