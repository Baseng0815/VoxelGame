#include "../include/Math/Cuboid.hpp"
#include <entt/entt.hpp>
#include <vector>

class Collision {
  private:
    static Math::Cuboid getCollision(entt::registry& registry, entt::entity entity);

  public:
    static bool checkCollision(entt::registry& registry, entt::entity first, entt::entity second);

    static Math::Cuboid getBlockCollision(glm::vec3 blockPos);

    static glm::vec3 getMTV(const Math::Cuboid& first, const Math::Cuboid& secnd);
    static std::vector<glm::vec3> getTVs(const Math::Cuboid& first, const Math::Cuboid& secnd);
    static std::vector<glm::vec3> getBlockTVs(const glm::vec3 block, const Math::Cuboid& collision);

    static std::vector<Math::Cuboid> getBlockCollisions(const entt::registry* registry, const Math::Cuboid& blocks);
};