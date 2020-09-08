#pragma once

#include "System.h"

struct PlayerComponent;
struct TransformationComponent;
struct VelocityComponent;

class PhysicsSystem : public System {
private:
    void _update(int dt) override;

    void updatePlayer(float dt, PlayerComponent& player, TransformationComponent& transform, VelocityComponent& velocity) const;
    static void updateEntity(float dt, TransformationComponent& transform, VelocityComponent& velocity);

  public:
    PhysicsSystem(entt::registry& registry);
};
