#include "../../include/Systems/PhysicsSystem.h"

#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/TransformationComponent.h"
#include "../../include/Components/VelocityComponent.h"

#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"
#include "../../include/World.h"

#include <iostream>

PhysicsSystem::PhysicsSystem(Registry_T &registry)
    : System {registry, 0}
{ }

void PhysicsSystem::_update(int millis) {    
    float dt = millis / 1000.f;

    m_registry.view<PlayerComponent, TransformationComponent, VelocityComponent>().each(
        [&](PlayerComponent& player, TransformationComponent& transform, VelocityComponent& velocity) {
            updatePlayer(dt, player, transform, velocity);
        }
        );

    m_registry.view<TransformationComponent, VelocityComponent>(entt::exclude<PlayerComponent>).each(
        [&](TransformationComponent& transform, VelocityComponent& velocity) {
            updateEntity(dt, transform, velocity);
        }
        );
}

void PhysicsSystem::updatePlayer(float dt, PlayerComponent& player, TransformationComponent& transform, VelocityComponent& velocity) const {    
    if (!World::chunkCreated(Utility::GetChunk(transform.getPosition())))
        return;

    glm::vec3 oldPlayerPos = transform.getPosition();
    glm::ivec2 oldChunk = glm::ivec2(
        (int)std::floor(oldPlayerPos.x / Configuration::CHUNK_SIZE),
        (int)std::floor(oldPlayerPos.z / Configuration::CHUNK_SIZE));

    if(player.isFalling) {
        float g = -9.8f;

        velocity.velocity.y = glm::min(dt * g + velocity.velocity.y, 10.0f);    
    }    

    transform.move(dt * velocity.velocity);

    glm::vec3 newPlayerPos = transform.getPosition();
    glm::ivec2 newChunk = glm::ivec2(
        (int)std::floor(newPlayerPos.x / Configuration::CHUNK_SIZE),
        (int)std::floor(newPlayerPos.z / Configuration::CHUNK_SIZE));

    if(newChunk != oldChunk)    {
        EnterChunkEvent e {nullptr, oldChunk.x, oldChunk.y, newChunk.x, newChunk.y};
        EventDispatcher::raiseEvent(e);
    }    

    // update is falling
    player.isFalling =  !World::getBlock(&m_registry, glm::floor(newPlayerPos + glm::vec3(0, -1, 0))).isSolid();    
}

void PhysicsSystem::updateEntity(float dt, TransformationComponent& transform, VelocityComponent& velocity) {
    // position
    glm::vec3 ds = dt * velocity.velocity;

    transform.move(ds);

    // rotation
    glm::vec3 axis = glm::normalize(velocity.angularVelocity);
    float a = glm::length(dt * velocity.angularVelocity);

    glm::vec3 dr = Utility::getEuler(axis, a);
    transform.rotate(dr);
}
