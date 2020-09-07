#pragma once

#include "System.h"

struct PlayerComponent;
struct TransformationComponent;
struct VelocityComponent;

class PhysicsSystem : public System {
private:
    void _update(int dt) override;

    static void updatePlayer(float dt, PlayerComponent& player, TransformationComponent& transform, VelocityComponent& velocity);
    static void updateEntity(float dt, TransformationComponent& transform, VelocityComponent& velocity);

  public:
    PhysicsSystem(entt::registry& registry);
};
