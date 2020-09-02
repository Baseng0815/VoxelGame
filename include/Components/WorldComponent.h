#include <glm/glm.hpp>
#include <unordered_map>
#include <entt/entt.hpp>
#include "../Utility.h"

struct Block;

// TODO remove logic from component and put the chunksLookup somewhere else,
// maybe remove completely if deemed unnecessary
class WorldComponent {
    private:
        std::unordered_map<glm::vec2, entt::entity, HashFunction> chunksLookup = std::unordered_map<glm::vec2, entt::entity, HashFunction>();;

    public:
        unsigned int worldID = 0;

        entt::entity getChunk(glm::vec2 chunk) const;
        bool chunkCreated(glm::vec2 chunk) const;

        Block getBlock(const entt::registry* registry, glm::vec3 position) const;
        void setBlock(entt::registry* registry, glm::vec3 position, Block block);

        void addChunk(entt::entity entity, glm::vec2 position);
        void removeChunk(entt::entity entity);
};
