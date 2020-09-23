#pragma once

#include "System.hpp"
#include <glm/glm.hpp>

#include "../Math/Cuboid.hpp"

struct PlayerComponent;
struct CameraComponent;
struct VelocityComponent;
struct TransformationComponent;

struct BlockCollisionEvent;

class PhysicsSystem : public System {
  private:
    CallbackHandle<const BlockCollisionEvent&> m_blockCollisionHandle;

    void _update(int dt) override;

    void updatePlayer(float dt, PlayerComponent& player, TransformationComponent& transform,
                      VelocityComponent& velocity, CameraComponent &camera) const;

    void handleBlockCollision(entt::entity entity, const glm::vec3 &block) const;

    // returns true if entity was moved
    static bool applyVelocities(float dt, TransformationComponent &transform, const VelocityComponent &velocity);

  public:
    PhysicsSystem(entt::registry& registry);
};
