#pragma once
#include "System.h"

struct PlayerComponent;
struct CameraComponent;
struct TransformationComponent;

class CollisionSystem : public System {
private:
    void _update(int dt) override;

    static void updatePlayerLookAtBlock(PlayerComponent& player, TransformationComponent& transform, CameraComponent& camera);
public:
    CollisionSystem(entt::registry* registry);
};