#include "../../include/Systems/PlayerMovementSystem.h"

#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/VelocityComponent.h"
#include "../../include/Components/CameraComponent.h"

#include "../../include/Utility.h"

#include <GLFW/glfw3.h>
#include <iostream>

PlayerMovementSystem::PlayerMovementSystem(Registry_T& registry)
    : System(registry, 10) {
}

void PlayerMovementSystem::_update(int dt) {    
    m_registry.view<PlayerComponent, VelocityComponent, CameraComponent>().each(
        [&](PlayerComponent& player, VelocityComponent& vel, CameraComponent& camera) {
            updatePlayerSpeed(player, vel, camera);
        }
    );    
}

void PlayerMovementSystem::updatePlayerSpeed(PlayerComponent& player, VelocityComponent& velocity, CameraComponent& camera) const {
    glm::vec3 playerMovementDir = player.xAxisInput * camera.front_noY + player.yAxisInput * camera.right;    

    if(player.isFalling) {
        playerMovementDir.y = 0;
    }
    else {
        playerMovementDir.y = player.zAxisInput;
    }    
    
    if(glm::length(playerMovementDir) != 0) {
        velocity.velocity = player.maxMovementSpeed * glm::normalize(playerMovementDir);        
    }    
    else {
        velocity.velocity.x *= 0.6;
        velocity.velocity.z *= 0.6;

        if(!player.isFalling)
            velocity.velocity.y *= 0.6;
    }
}