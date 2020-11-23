#include "../../include/Systems/PlayerMovementSystem.hpp"

#include "../../include/Components/CameraComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/VelocityComponent.hpp"
#include "../../include/Components/RigidBodyComponent.hpp"

#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/Utility.hpp"

#include <iostream>

void PlayerMovementSystem::handlePlayerMoved(const EntityMovedEvent &e) const {
    CameraComponent &camera = m_registry.get<CameraComponent>(e.entity);
    camera.viewMatrixOutdated = true;
}

void PlayerMovementSystem::_update(int dt) {
    entt::entity player = m_registry.view<PlayerComponent>().front();
    PlayerComponent &playerComponent = m_registry.get<PlayerComponent>(player);
    VelocityComponent &velocityComponent = m_registry.get<VelocityComponent>(player);
    const CameraComponent &cameraComponent = m_registry.get<CameraComponent>(player);
    const RigidBodyComponent &rigidBody = m_registry.get<RigidBodyComponent>(player);

    updatePlayerSpeed(playerComponent, velocityComponent, cameraComponent, rigidBody);
}

void PlayerMovementSystem::updatePlayerSpeed(PlayerComponent &player, VelocityComponent &velocity, const CameraComponent &camera, const RigidBodyComponent &rigidBody) const {
    glm::vec3 playerInput {0.f};

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

        if (velocity.velocity.y > player.maxMovementSpeed) {
            velocity.velocity.y = player.maxMovementSpeed;
        }
    }
    else {
        if (velocity.velocity.y == 0) {
            velocity.velocity *= 0.2f;
        }
        else {
            velocity.velocity.x *= 0.8f;
            velocity.velocity.z *= 0.8f;
        }
    }
}

PlayerMovementSystem::PlayerMovementSystem(Registry_T &registry)
    : System(registry, 0) {

    m_playerMovedHandle = EventDispatcher::onEntityMoved.subscribe([&](const EntityMovedEvent &e) {
        // if entity has PleyerComponent
        if (m_registry.any<PlayerComponent>(e.entity)) {
            handlePlayerMoved(e);
        }
    });
}
