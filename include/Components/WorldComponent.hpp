#include <glm/glm.hpp>
#include <unordered_map>
#include <entt/entt.hpp>

#include "../Utility.hpp"

struct Block;

// TODO remove logic from component and put the chunksLookup somewhere else,
// maybe remove completely if deemed unnecessary
class WorldComponent {
    private:
        std::unordered_map<glm::vec2, entt::entity, Utility::HashFunctionVec2> chunksLookup = std::unordered_map<glm::vec2, entt::entity, Utility::HashFunctionVec2>();;

    public:
        unsigned int worldID = 0;

        entt::entity getChunk(const glm::vec2 &chunk) const;
        bool chunkCreated(const glm::vec2 &chunk) const;

        Block getBlock(const entt::registry* registry, const glm::vec3 &position) const;
        void setBlock(entt::registry* registry, const glm::vec3 &position, Block block);

        void addChunk(entt::entity entity, const glm::vec2 &position);
        void removeChunk(entt::entity entity);
};
