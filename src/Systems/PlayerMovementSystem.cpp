#include "../../include/Systems/PlayerMovementSystem.h"

#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/VelocityComponent.h"

#include <GLFW/glfw3.h>

PlayerMovementSystem::PlayerMovementSystem(entt::registry* registry, int updateDelay)
    : System(registry, updateDelay) {
}

void PlayerMovementSystem::_update(int dt) {
    m_registry->view<PlayerComponent, VelocityComponent>().each(
        [&](PlayerComponent& player, VelocityComponent& vel) {
            updatePlayerSpeed(player, vel);
        }
    );
}

void PlayerMovementSystem::updatePlayerSpeed(PlayerComponent& player, VelocityComponent& velocity) {
    velocity.velocity.x = player.maxMovementSpeed * player.xAxisInput;
    velocity.velocity.y = player.maxMovementSpeed * player.yAxisInput;

    if(player.isFalling) {
        velocity.velocity.z = 0;
    }
    else {
        velocity.velocity.z = player.maxMovementSpeed * player.zAxisInput;
    }
}