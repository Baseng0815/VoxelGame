#pragma once

#include "System.hpp"
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "../Math/Cuboid.hpp"

struct PlayerComponent;
struct CameraComponent;
struct VelocityComponent;
struct TransformationComponent;
struct RigidBodyComponent;

struct BlockCollisionEvent;

class PhysicsSystem : public System {
  private:
    CallbackHandle<const BlockCollisionEvent &> m_blockCollisionHandle;

    void _update(int dt) override;

    void handleBlockCollision(entt::entity entity, const glm::vec3 &block) const;

    // raises EntityMovedEvent if entity was moved
    void applyVelocities(float dt, const entt::entity &entity, TransformationComponent &transform, const VelocityComponent &velocity) const;

    static void applyAccelerations(float dt, VelocityComponent &velocity, const RigidBodyComponent &rigidBody);

    void updateFalling(RigidBodyComponent &rigidBody, const TransformationComponent &transform, const VelocityComponent &velocity) const;

  public:
    PhysicsSystem(entt::registry &registry);
};
