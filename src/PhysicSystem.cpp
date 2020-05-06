#include "../include/PhysicSystem.h"
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

PhysicSystem::PhysicSystem(SystemManager* systemManager, SharedContext* context)
	: System(systemManager, context) {
}

void PhysicSystem::update(int dt) {
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

		glm::vec3 dw = (float)dt * glm::vec3(); // angular acceleration

		// apply transformations
		// ds = dt * v

		glm::vec3 ds = (float)dt * rigidBody.velocity;
		transformation.position += ds;

		//				.
		// dphi = dt *	w

		glm::vec3 dphi = (float)dt * rigidBody.angularVelocity;
		transformation.rotation += dphi;

		detectCollision(entity, rigidBody, transformation);
	}
}

void PhysicSystem::detectCollision(entt::entity entity, RigidBodyComponent& rigidBody, TransformationComponent& transform) {
	auto view = m_systemManager->getRegistry().view<TransformationComponent, RigidBodyComponent>();


	for (auto e : view) {
		if (e == entity)
			continue;

		RigidBodyComponent& otherRigidBody = view.get<RigidBodyComponent>(e);
		TransformationComponent& otherTransform = view.get<TransformationComponent>(e);

		// check hitbox overlapping
		if (rigidBody.checkCollision(transform.getModelMatrix(), otherRigidBody, otherTransform.getModelMatrix())) {
			collisions.push_back(std::make_pair(entity, e));
		}
	}
}

PhysicSystem::~PhysicSystem() {

}
