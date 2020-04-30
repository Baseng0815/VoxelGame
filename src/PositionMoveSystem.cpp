#include "../include/PositionMoveSystem.h"

#include "../include/Components/TransformationComponent.h"
#include "../include/Components/VelocityComponent.h"

PositionMoveSystem::PositionMoveSystem(SystemManager* systemManager)
    : System(systemManager) {}

void PositionMoveSystem::update(int dt) {
    m_systemManager->getRegistry().view<TransformationComponent, VelocityComponent>().each(
            [=](auto& transformation, auto& velocity) {
        transformation.position += velocity.velocity * dt;
    });
}
