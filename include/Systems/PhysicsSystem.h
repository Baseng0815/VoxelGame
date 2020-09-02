#pragma once

#include "System.h"

struct PlayerComponent;
struct TransformationComponent;
struct VelocityComponent;

class PhysicsSystem : public System {
private:
    void _update(int dt) override;

    static void updatePlayer(PlayerComponent& player, TransformationComponent& transform, VelocityComponent& velocity, float dt);

public:
    PhysicsSystem(entt::registry& registry);
};
