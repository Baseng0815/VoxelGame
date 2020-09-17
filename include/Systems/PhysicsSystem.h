#pragma once

#include "System.h"
#include <glm/glm.hpp>

struct PlayerComponent;
struct TransformationComponent;
struct VelocityComponent;
struct BlockCollisionEvent;

class PhysicsSystem : public System {
  private:
    CallbackHandle<const BlockCollisionEvent&> m_blockCollisionHandle;

    void _update(int dt) override;

    void updatePlayer(float dt, PlayerComponent& player, TransformationComponent& transform,
                      VelocityComponent& velocity) const;
    void handleBlockCollision(entt::entity entity, glm::vec3 block) const;

    static void applyVelocities(float dt, TransformationComponent& transform, VelocityComponent& velocity);

  public:
    PhysicsSystem(entt::registry& registry);
};
