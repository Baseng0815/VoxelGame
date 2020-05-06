#include "../include/PhysicsSystem.h"
#include "../include/Components/ChunkComponent.h"
#include "../include/Components/RigidBodyComponent.h"
#include "../include/Components/TransformationComponent.h"
#include "../include/SystemManager.h"
#include "../include/Block.h"

#include <glm/glm.hpp>

//			^ y
//			|
//			|
//			|
//			---------> x
//		   /
//		  /
//		 /
//		v z

void PhysicsSystem::_update(int dt) {
    auto view = m_systemManager->getRegistry().view<TransformationComponent, RigidBodyComponent>();

    glm::vec3 gravitationAcceleration = glm::vec3(0, -9.81f, 0);

    for (auto entity : view) {

        RigidBodyComponent& rigidBody = view.get<RigidBodyComponent>(entity);
        TransformationComponent& transformation = view.get<TransformationComponent>(entity);

        // apply forces
        // gravitation
        // dv = dt * a = dt * F/m

        glm::vec3 dv = (float)dt * gravitationAcceleration;
        rigidBody.velocity += dv;

        // apply transformations
        // ds = dt * v

        glm::vec3 ds = (float)dt * rigidBody.velocity;
        transformation.position += ds;


    }
}

PhysicsSystem::PhysicsSystem(SystemManager* systemManager)
    : System(systemManager, 0) {}

