#include "../../include/Systems/PhysicsSystem.h"
#include "../../include/Systems/SystemManager.h"

#include "../../include/Components/CameraComponent.h"
#include "../../include/Components/ChunkComponent.h"
#include "../../include/Components/RigidBodyComponent.h"
#include "../../include/Components/TransformationComponent.h"
#include "../../include/Components/VelocityComponent.h"
#include "../../include/Components/WorldComponent.h"

#include "../../include/Block.h"
#include "../../include/Configuration.h"
#include "../../include/Collision.h"
#include "../../include/Event.h"
#include "../../include/EventDispatcher.h"

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
	if (!m_systemManager->physicsActive)
		return;

	float dtSec = dt / 1000.f;

	auto& registry = m_systemManager->getRegistry();

	auto worldView = registry.view<WorldComponent>();

	WorldComponent& world = worldView.get(worldView.front());

	// update velocities
	{
		auto view = registry.view<TransformationComponent, RigidBodyComponent, VelocityComponent>();

		glm::vec3 gravitationAcceleration = glm::vec3(0, -Configuration::getFloatValue("G_CONSTANT"), 0);

		for (auto entity : view) {
			RigidBodyComponent& rigidBody = view.get<RigidBodyComponent>(entity);
			TransformationComponent& transformation = view.get<TransformationComponent>(entity);
			VelocityComponent& velocity = view.get<VelocityComponent>(entity);

			// apply forces
			// gravitation
			// dv = dt * a = dt * F/m
			// dt in milliseconds

			bool isCamera = registry.has<CameraComponent>(entity);

			if (isCamera) {
				CameraComponent& camera = registry.get<CameraComponent>(entity);

				if (camera.isFlying) {

				}				
				else if (camera.isFalling) {
					glm::vec3 dv = (float)dtSec * gravitationAcceleration;

					camera.relVelocity += dv;

					camera.isFalling = world.getBlock(registry, glm::ivec3(transformation.position.x, transformation.position.y - 1.5f, transformation.position.z)).type == BlockType::BLOCK_AIR;
					if (!camera.isFalling) {
						camera.relVelocity.y = 0;
					}

					if (camera.relVelocity.y < -Configuration::getFloatValue("MAX_FALL_SPEED")) {
						camera.relVelocity.y = -Configuration::getFloatValue("MAX_FALL_SPEED");
					}
				}
				else {
					camera.isFalling = world.getBlock(registry, glm::ivec3(transformation.position.x, transformation.position.y - 1.5f, transformation.position.z)).type == BlockType::BLOCK_AIR;
					if (camera.relVelocity.y < 0)
						camera.relVelocity.y = 0;
				}
			
				//std::cout << "x: " << camera.relVelocity.x << " y: " << camera.relVelocity.y << " z: " << camera.relVelocity.z << " is flying: " << camera.isFlying << std::endl;

			}
			else {
				glm::vec3 dv = (float)dtSec * gravitationAcceleration;

				if (velocity.velocity.y < -Configuration::getFloatValue("MAX_FALL_SPEED")) {
					dv.y = 0;
					velocity.velocity.y = -Configuration::getFloatValue("MAX_FALL_SPEED");
				}

				velocity.velocity += dv;
			}
		}
	}

	// update transformations
	{
		auto view = registry.view<TransformationComponent, VelocityComponent>();

		for (auto entity : view) {
			TransformationComponent& transformation = view.get<TransformationComponent>(entity);
			VelocityComponent& velocity = view.get<VelocityComponent>(entity);

			bool isCamera = registry.has<CameraComponent>(entity);
			//glm::vec3 oldPos = transformation.position;		
			//glm::vec3 newPos = oldPos;

			if (isCamera) {
				CameraComponent& camera = registry.get<CameraComponent>(entity);

				int prevChunkX = transformation.position.x / Configuration::CHUNK_SIZE;
				int prevChunkZ = transformation.position.z / Configuration::CHUNK_SIZE;

				velocity.velocity = camera.relVelocity.x * camera.right
					+ camera.relVelocity.y * glm::vec3(0, 1, 0)
					+ camera.relVelocity.z * camera.front_noY;

				transformation.position += (velocity.velocity) * (float)dtSec * Configuration::getFloatValue("CAMERA_MOVE_SPEED");

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

				if (length(velocity.velocity) != 0) {
					movedObjects.push_back(entity);
					//newPos = transformation.position;
				}
			}
			else {
				transformation.position += velocity.velocity * (float)dtSec;

				if (length(velocity.velocity) != 0) {
					movedObjects.push_back(entity);
					//newPos = transformation.position;
				}
			}

			//if (newPos != oldPos) {				
			//	EntityMovedEvent e;
			//	e.entity = entity;
			//	e.oldPos = oldPos;
			//	e.newPos = newPos;

			//	EventDispatcher::raiseEvent(&e);
			//}
		}
	}

	solveBlockCollisions();
}

void PhysicsSystem::solveBlockCollisions() {
	auto& registry = m_systemManager->getRegistry();
	auto chunksView = m_systemManager->getRegistry().view<ChunkComponent>();
	auto worldView = m_systemManager->getRegistry().view<WorldComponent>();

	WorldComponent& world = worldView.get<WorldComponent>(worldView.front());
	for (auto worldEntity : worldView) {
		auto& worldComponent = worldView.get<WorldComponent>(worldEntity);
		if (worldComponent.worldID == 0)
			world = worldComponent;
	}

	for (auto entity : movedObjects) {
		TransformationComponent& transformation = registry.get<TransformationComponent>(entity);
		RigidBodyComponent& rigidBody = registry.get<RigidBodyComponent>(entity);

		BoxCollision collision = BoxCollision(*rigidBody.collision);

		std::vector<glm::vec3> affectedBlocks = collision.getAffectedBlocks(transformation.position);
		glm::vec3 moveOutVec = glm::vec3();
		bool hasCollision = false;

		for (auto it = affectedBlocks.begin(); it != affectedBlocks.end(); it++) {
			glm::vec3 blockPos = (*it);

			// check block has collision
			int y = (int)blockPos.y % Configuration::CHUNK_HEIGHT;

			if (y <= 1 || y > Configuration::CHUNK_HEIGHT)
				hasCollision = false;
			else {
				hasCollision = world.getBlock(registry, blockPos).type != BlockType::BLOCK_AIR;
			}

			// break collision detection
			if (!hasCollision)
				continue;

			// get block collision
			BoxCollision blockCollision = BoxCollision(glm::vec3(-0.5f, -0.5f, -0.5f), 1, 1, 1);
			blockCollision.dynamic = false;

			checkAndHandleCollisions(collision, blockCollision, transformation.position, blockPos);
		}
	}

	movedObjects.clear();
}

void PhysicsSystem::checkAndHandleCollisions(const BoxCollision& collisionA, const BoxCollision& collisionB,
	glm::vec3& posA, glm::vec3& posB) const {

	if (Collision::intersects(&collisionA, posA, &collisionB, posB)) {
		std::vector<glm::vec3> translationVectors = Collision::getTranslationVectors(&collisionA, posA, &collisionB, posB);

		glm::vec3 mtv = glm::vec3();
		float minLenght = FLT_MAX;

		for (int i = 0; i < 6; i++) {
			if (glm::length(translationVectors[i]) < minLenght) {
				minLenght = glm::length(translationVectors[i]);
				mtv = translationVectors[i];
			}
		}

		if (collisionA.dynamic && collisionB.dynamic) {
			posA += 0.55f * mtv;
			posB += 0.55f * -mtv;
		}
		else if (collisionA.dynamic) {
			posA += 1.1f * mtv;
		}
		else if (collisionB.dynamic) {
			posB += 1.1f * -mtv;
		}
	}
}

PhysicsSystem::PhysicsSystem(SystemManager* systemManager)
	: System(systemManager, 0) {
}

