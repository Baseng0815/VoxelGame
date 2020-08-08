#pragma once
#include "System.h"

struct CameraComponent;
struct CollisionComponent;
struct PlayerComponent;
struct TransformationComponent;

class CollisionSystem : public System {
private:
    void _update(int dt) override;

    void updatePlayerLookAtBlock(PlayerComponent& player, TransformationComponent& transform, CameraComponent& camera) const;
    void checkCollisions(entt::entity first, entt::entity secnd);
public:
    CollisionSystem(entt::registry* registry);
};