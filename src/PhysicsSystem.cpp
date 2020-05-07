#include "../include/PhysicsSystem.h"
#include "../include/SystemManager.h"

#include "../include/Components/CameraComponent.h"
#include "../include/Components/ChunkComponent.h"
#include "../include/Components/RigidBodyComponent.h"
#include "../include/Components/TransformationComponent.h"
#include "../include/Components/VelocityComponent.h"

#include "../include/Block.h"
#include "../include/Configuration.h"
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
	float dtSec = dt / 1000.f;

	auto& registry = m_systemManager->getRegistry();

	// update velocities
	{
		auto view = registry.view<TransformationComponent, RigidBodyComponent>();

		glm::vec3 gravitationAcceleration = glm::vec3(0, -Configuration::getFloatValue("G_CONSTANT"), 0);

		for (auto entity : view) {

			RigidBodyComponent& rigidBody = view.get<RigidBodyComponent>(entity);
			TransformationComponent& transformation = view.get<TransformationComponent>(entity);

			// apply forces
			// gravitation
			// dv = dt * a = dt * F/m
			// dt in milliseconds

			glm::vec3 dv = (float)dtSec / 1000.0f * gravitationAcceleration;
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

				transformation.position += velocity.velocity * (float)dtSec * Configuration::getFloatValue("CAMERA_MOVE_SPEED");

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
			else {
				transformation.position += velocity.velocity * (float)dtSec / 1000.f;
			}

			if (length(velocity.velocity) != 0) {
				movedObjects.push_back(entity);
			}
		}
	}

	solveBlockCollisions();
}

void PhysicsSystem::solveBlockCollisions() {
	auto& registry = m_systemManager->getRegistry();

	auto view = m_systemManager->getRegistry().view<TransformationComponent, RigidBodyComponent>();
	auto chunksView = m_systemManager->getRegistry().view<ChunkComponent>();

	for (auto entity : movedObjects) {
		TransformationComponent& transformation = registry.get<TransformationComponent>(entity);
		RigidBodyComponent& rigidBody = registry.get<RigidBodyComponent>(entity);

		auto affectedBlocks = rigidBody.collision->applyTransformation(transformation).getAffectedBlocks();

		for (auto it = affectedBlocks.begin(); it != affectedBlocks.end(); it++) {
			glm::vec3 pos = *it;

			int chunkX = pos.x / Configuration::CHUNK_SIZE;
			int chunkZ = pos.z / Configuration::CHUNK_SIZE;

			for (auto e : chunksView) {
				ChunkComponent& chunkComp = chunksView.get(e);

				if (chunkComp.chunkX == chunkX && chunkComp.chunkZ == chunkZ) {
					int x = (int)pos.x % Configuration::CHUNK_SIZE;
					int y = (int)pos.y;
					int z = (int)pos.z % Configuration::CHUNK_SIZE;

					if (x < 0)
						x = Configuration::CHUNK_SIZE + x;

					if (z < 0)
						z = Configuration::CHUNK_SIZE + z;

					if (y < 0 || y >= Configuration::CHUNK_HEIGHT)
						continue;

					if (!chunkComp.threadActiveOnSelf) {
						std::unique_lock<std::mutex> blockLock(*chunkComp.blockMutex);

						if (chunkComp.blocks[x][y][z].type != BlockType::BLOCK_AIR) {
							transformation.position.y += 1;
						}

						blockLock.unlock();

						continue;
					}
				}
			}
		}
	}

	movedObjects.clear();
}

PhysicsSystem::PhysicsSystem(SystemManager* systemManager)
	: System(systemManager, 0) {
}

