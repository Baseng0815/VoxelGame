#include "../include/PhysicsSystem.h"
#include "../include/SystemManager.h"

#include "../include/Components/CameraComponent.h"
#include "../include/Components/ChunkComponent.h"
#include "../include/Components/RigidBodyComponent.h"
#include "../include/Components/TransformationComponent.h"
#include "../include/Components/VelocityComponent.h"

#include "../include/Block.h"
#include "../include/Event.h"
#include "../include/EventDispatcher.h"

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
	auto& registry = m_systemManager->getRegistry();

	// update velocities
	{
		auto view = registry.view<TransformationComponent, RigidBodyComponent>();

		glm::vec3 gravitationAcceleration = glm::vec3(0, -9.81f, 0);

		for (auto entity : view) {

			RigidBodyComponent& rigidBody = view.get<RigidBodyComponent>(entity);
			TransformationComponent& transformation = view.get<TransformationComponent>(entity);

			// apply forces
			// gravitation
			// dv = dt * a = dt * F/m
			// dt in milliseconds

			glm::vec3 dv = (float)dt / 1000.0f * gravitationAcceleration;
			rigidBody.velocity += dv;
		}
	}

	// update transformations
	{
		auto view = registry.view<TransformationComponent, VelocityComponent>();

		for (auto entity : view) {
			TransformationComponent& transformation = view.get<TransformationComponent>(entity);
			VelocityComponent& velocity = view.get<VelocityComponent>(entity);

			bool isCamera = registry.has<CameraComponent>(entity);

			if (isCamera) {
				int prevChunkX = transformation.position.x / Configuration::CHUNK_SIZE;
				int prevChunkZ = transformation.position.z / Configuration::CHUNK_SIZE;

				transformation.position += velocity.velocity * (float)dt * Configuration::getFloatValue("CAMERA_MOVE_SPEED");

				std::cout << velocity.velocity.x << " " << velocity.velocity.y << " " << velocity.velocity.z << std::endl;
				std::cout << "\t" << transformation.position.x << " " << transformation.position.y << " " << transformation.position.z << std::endl;

				int newChunkX = transformation.position.x / Configuration::CHUNK_SIZE;
				int newChunkZ = transformation.position.z / Configuration::CHUNK_SIZE;

				if (newChunkX != prevChunkX || newChunkZ != prevChunkZ) {
					EnterChunkEvent e;
					e.oldX = prevChunkX;
					e.oldZ = prevChunkZ;
					e.newX = newChunkX;
					e.newZ = newChunkZ;

					EventDispatcher::raiseEvent(&e);
				}
			}
		}
	}
}

PhysicsSystem::PhysicsSystem(SystemManager* systemManager)
	: System(systemManager, 0) {
}

