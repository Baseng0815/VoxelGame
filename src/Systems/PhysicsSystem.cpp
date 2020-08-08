#include "../../include/Systems/PhysicsSystem.h"

#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/TransformationComponent.h"
#include "../../include/Components/VelocityComponent.h"

#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"

#include <iostream>

PhysicsSystem::PhysicsSystem(entt::registry* registry)
    : System(registry, 0) {
}

void PhysicsSystem::_update(int millis) {
    float dt = millis / 1000.f;

    m_registry->view<PlayerComponent, TransformationComponent, VelocityComponent>().each(
        [&](PlayerComponent& player, TransformationComponent& transform, VelocityComponent& velocity) {
            updatePlayer(player, transform, velocity, dt);
        }
    );
}

void PhysicsSystem::updatePlayer(PlayerComponent& player, TransformationComponent& transform, VelocityComponent& velocity, float dt) {
    glm::vec3 oldPlayerPos = transform.getPosition();
    glm::ivec2 oldChunk = glm::ivec2(
        (int)oldPlayerPos.x / CHUNK_SIZE,
        (int)oldPlayerPos.z / CHUNK_SIZE
    );
    
    transform.move(dt * velocity.velocity);    
    glm::vec3 newPlayerPos = transform.getPosition();
    glm::ivec2 newChunk = glm::ivec2(
        (int)newPlayerPos.x / CHUNK_SIZE,
        (int)newPlayerPos.z / CHUNK_SIZE
    );
    
    if(newChunk != oldChunk)    {
        EnterChunkEvent e = EnterChunkEvent(nullptr, oldChunk.x, oldChunk.y, newChunk.x, newChunk.y);
        EventDispatcher::raiseEvent(e);

    }
    
    //std::cout << "chunk x:" << newChunk.x << " y: " << newChunk.y << std::endl;
}
