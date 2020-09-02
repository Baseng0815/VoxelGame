#pragma once
#include "System.h"

struct PlayerComponent;
struct VelocityComponent;
struct CameraComponent;

class PlayerMovementSystem : public System {
private:
    entt::entity playerEntity;

    void _update(int dt) override;

    void updatePlayerSpeed(PlayerComponent& player, VelocityComponent& velocity, CameraComponent& camera) const;
public:
    PlayerMovementSystem(Registry_T& registry);
};