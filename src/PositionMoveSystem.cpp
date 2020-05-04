#include "../include/PositionMoveSystem.h"

#include "../include/SystemManager.h"

#include "../include/Components/TransformationComponent.h"
#include "../include/Components/VelocityComponent.h"

void PositionMoveSystem::_update(int dt) {
    m_systemManager->getRegistry().view<TransformationComponent, VelocityComponent>().each(
            [=](auto& transformation, auto& velocity) {
        transformation.position += velocity.velocity * (float)dt;
    });
}

PositionMoveSystem::PositionMoveSystem(SystemManager* systemManager)
    : System(systemManager, 0) {}
