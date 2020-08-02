#pragma once
#include "System.h"

struct PlayerComponent;
struct VelocityComponent;

class PlayerMovementSystem : public System {
private:
    entt::entity playerEntity;

    void _update(int dt) override;

    void updatePlayerSpeed(PlayerComponent& player, VelocityComponent& velocity);
public:
    PlayerMovementSystem(entt::registry* registry, int updateDelay);


};