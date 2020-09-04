#include "../../include/Systems/PhysicsSystem.h"

#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/TransformationComponent.h"
#include "../../include/Components/VelocityComponent.h"

#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"
#include "../../include/World.h"

#include <iostream>

PhysicsSystem::PhysicsSystem(entt::registry& registry)
    : System(registry, 0) {
}

void PhysicsSystem::_update(int millis) {    
    float dt = millis / 1000.f;

    m_registry.view<PlayerComponent, TransformationComponent, VelocityComponent>().each(
        [&](PlayerComponent& player, TransformationComponent& transform, VelocityComponent& velocity) {
            updatePlayer(player, transform, velocity, dt);
        }
    );
}

void PhysicsSystem::updatePlayer(PlayerComponent& player, TransformationComponent& transform, VelocityComponent& velocity, float dt) {    
    if (!World::chunkCreated(GetChunk(transform.getPosition(), glm::vec3())))
        return;

    glm::vec3 oldPlayerPos = transform.getPosition();
    glm::ivec2 oldChunk = glm::ivec2(
        (int)oldPlayerPos.x / Configuration::CHUNK_SIZE,
        (int)oldPlayerPos.z / Configuration::CHUNK_SIZE);

    if(player.isFalling) {
        float g = -9.8f;

        velocity.velocity.y = glm::min(dt * g + velocity.velocity.y, 10.0f);    
    }

    transform.move(dt * velocity.velocity);    
    glm::vec3 newPlayerPos = transform.getPosition();
    glm::ivec2 newChunk = glm::ivec2(
        (int)newPlayerPos.x / Configuration::CHUNK_SIZE,
        (int)newPlayerPos.z / Configuration::CHUNK_SIZE);

    if(newChunk != oldChunk)    {
        EnterChunkEvent e = EnterChunkEvent(nullptr, oldChunk.x, oldChunk.y, newChunk.x, newChunk.y);
        EventDispatcher::raiseEvent(e);
    }    
}
