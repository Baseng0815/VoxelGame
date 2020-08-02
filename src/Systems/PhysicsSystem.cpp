#include "../../include/Systems/PhysicsSystem.h"

#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/TransformationComponent.h"
#include "../../include/Components/VelocityComponent.h"

PhysicsSystem::PhysicsSystem(entt::registry* registry) 
    : System(registry, 0) {
}

void _update(int dt) {
    m_registry->view<PlayerComponent, TransformationComponent, VelocityComponent>().each(
        [&](PlayerComponent& player, TransformationComponent& transform, VelocityComponent& velocity) {
            updatePlayer(player, transform, velocity, dt);
        }
    );    
}

void updatePlayer(PlayerComponent& player, TransformationComponent& transform, VelocityComponent& velocity, int dt) {
    transform += dt * velocity;
}