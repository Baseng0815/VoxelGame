#include "../include/Utility.h"
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <unordered_map>

struct Block;

class World {
  private:
    static std::unordered_map<glm::vec2, entt::entity, HashFunction> chunksLookup;

  public:    
    static entt::entity getChunk(glm::vec2 chunk);
    static bool chunkCreated(glm::vec2 chunk);
    
    static Block getBlock(const entt::registry* registry, glm::vec3 position);
    static void setBlock(entt::registry* registry, glm::vec3 position, Block block);

    static void addChunk(entt::entity entity, glm::vec2 position);
    static void removeChunk(entt::entity entity);
};