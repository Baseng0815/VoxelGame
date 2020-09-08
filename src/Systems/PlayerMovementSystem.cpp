#include "../../include/Systems/PlayerMovementSystem.h"

#include "../../include/Components/CameraComponent.h"
#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/VelocityComponent.h"

#include "../../include/Utility.h"

#include <GLFW/glfw3.h>
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
    }
    else {
        if(!player.isFalling)
            velocity.velocity *= 0.2f;
    }

    std::cout << player.isFalling << std::endl;    
    std::cout << velocity.velocity << std::endl;
}