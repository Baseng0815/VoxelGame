#include "../../include/Systems/PlayerMovementSystem.hpp"

#include "../../include/Components/CameraComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/VelocityComponent.hpp"

#include "../../include/Utility.hpp"

#include <iostream>

PlayerMovementSystem::PlayerMovementSystem(Registry_T& registry) : System(registry, 10) {
}

void PlayerMovementSystem::_update(int dt) {
    m_registry.view<PlayerComponent, VelocityComponent, CameraComponent>().each(
        [&](PlayerComponent& player, VelocityComponent& vel, CameraComponent& camera) {
            updatePlayerSpeed(player, vel, camera);
        });
}

void PlayerMovementSystem::updatePlayerSpeed(PlayerComponent& player, VelocityComponent& velocity,
                                             CameraComponent& camera) const {

    glm::vec3 playerInput = glm::vec3();    

    if (player.xAxisInput != 0) {
        playerInput += player.xAxisInput * camera.front_noY;
    }    
   
    if (player.yAxisInput != 0) {
        playerInput += player.yAxisInput * camera.right;
    }

    if (player.zAxisInput != 0) {
        playerInput += player.zAxisInput * glm::vec3(0, 1, 0);
    }    


    if (glm::length(playerInput) != 0) {
        glm::vec3 playerMovementDir = glm::normalize(playerInput);

        velocity.velocity += player.maxMovementSpeed * glm::normalize(playerMovementDir);

        float speed = sqrt(velocity.velocity.x * velocity.velocity.x + velocity.velocity.z * velocity.velocity.z);

        if (speed > player.maxMovementSpeed) {
            velocity.velocity.x *= player.maxMovementSpeed / speed;
            velocity.velocity.z *= player.maxMovementSpeed / speed;            
        }

        if(velocity.velocity.y > player.maxMovementSpeed) {
            velocity.velocity.y = player.maxMovementSpeed;
        }
    }
    else {
        if(!player.isFalling)
            velocity.velocity *= 0.2f;
    }    
}