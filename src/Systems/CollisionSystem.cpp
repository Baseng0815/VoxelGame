#include "../../include/Systems/CollisionSystem.h"

#include "../../include/Components/CameraComponent.h"
#include "../../include/Components/CollisionComponent.h"
#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/TransformationComponent.h"
#include "../../include/Components/WorldComponent.h"

#include "../../include/Ray.h"
#include "../../include/Plane.h"
#include "../../include/Utility.h"
#include "../../include/Events/Event.h"
#include "../../include/Events/EventDispatcher.h"

#include <utility>
#include <iostream>

CollisionSystem::CollisionSystem(Registry_T& registry)
	: System{ registry, 0 }
{
	m_cursorMoveHandle = EventDispatcher::onCursorMove.subscribe([this](const CursorEvent& e) {
		this->m_registry.view<PlayerComponent, TransformationComponent, CameraComponent>().each(
			[&](PlayerComponent& player, TransformationComponent& transform, CameraComponent& camera) {
				this->updatePlayerLookAtBlock(player, transform, camera);
			}
		);
		});
}

void CollisionSystem::_update(int dt)
{
	auto view = m_registry.view<TransformationComponent, CollisionComponent>();

	for (auto it = view.begin(); it != view.end(); it++) {
		for (auto jt = it; jt != view.end(); jt++) {
			if (it == jt)
				continue;

			checkCollisions(*it, *jt);
		}
	}
}

void CollisionSystem::updatePlayerLookAtBlock(PlayerComponent& player, TransformationComponent& transform, CameraComponent& camera) const
{
	Ray lookDirection = Ray(transform.getPosition() + camera.playerOffset, camera.front);

	glm::vec3 direction = lookDirection.getDirection();
	int startX = (direction.x >= 0 ? floor(lookDirection.origin.x) : ceil(lookDirection.origin.x));
	int startY = (direction.y >= 0 ? floor(lookDirection.origin.y) : ceil(lookDirection.origin.y));
	int startZ = (direction.z >= 0 ? floor(lookDirection.origin.z) : ceil(lookDirection.origin.z));

	int stepX = (int)(direction.x / abs(direction.x));
	int stepY = (int)(direction.y / abs(direction.y));
	int stepZ = (int)(direction.z / abs(direction.z));

	std::vector<glm::vec3> lookAtPositions = std::vector<glm::vec3>();
	int maxDist = 5;
	float dist = 0;

	int x = startX, y = startY, z = startZ;

	while (dist < maxDist) {
		float rx = direction.x == 0 ? INT_MAX : (x - lookDirection.origin.x) / direction.x;
		float ry = direction.y == 0 ? INT_MAX : (y - lookDirection.origin.y) / direction.y;
		float rz = direction.z == 0 ? INT_MAX : (z - lookDirection.origin.z) / direction.z;

		float minR = std::min(rx, std::min(ry, rz));

		glm::vec3 block = getBlockCoords(lookDirection.getPoint(minR));
		if (glm::length(block - lookDirection.origin) < maxDist) {
			lookAtPositions.push_back(block);
		}

		if (minR == rx) {
			x += stepX;
		}
		else if (minR == ry) {
			y += stepY;
		}
		else if (minR == rz) {
			z += stepZ;
		}

		dist = minR;
	}

	auto worldView = m_registry.view<WorldComponent>();
	WorldComponent& world = m_registry.get<WorldComponent>(worldView.front());

	for (std::vector<glm::vec3>::iterator it = lookAtPositions.begin(); it != lookAtPositions.end(); it++) {
		Block block = world.getBlock(&m_registry, *it);

		if (block.isSolid()) {
			player.lookAt = *it;

			break;
		}
	}
}

void CollisionSystem::checkCollisions(entt::entity first, entt::entity secnd)
{
	const TransformationComponent& firstTransform = m_registry.get<TransformationComponent>(first);
	const CollisionComponent& firstCollision = m_registry.get<CollisionComponent>(first);

	const TransformationComponent& secndTransform = m_registry.get<TransformationComponent>(secnd);
	const CollisionComponent& secndCollision = m_registry.get<CollisionComponent>(secnd);

	Cuboid c1 = firstCollision.getCuboid(firstTransform);
	Cuboid c2 = secndCollision.getCuboid(secndTransform);

	bool intersection = c1.intersects(c2);

	if (intersection) {
		// TODO: handle collision

		return;
	}	
}