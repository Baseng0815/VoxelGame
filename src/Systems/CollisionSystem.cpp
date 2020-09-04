#include "../../include/Systems/CollisionSystem.h"

#include "../../include/Components/CameraComponent.h"
#include "../../include/Components/CollisionComponent.h"
#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/TransformationComponent.h"
#include "../../include/Components/VelocityComponent.h"

#include "../../include/Events/Event.h"
#include "../../include/Events/EventDispatcher.h"
#include "../../include/Plane.h"
#include "../../include/Ray.h"
#include "../../include/Utility.h"
#include "../../include/World.h"

#include <iostream>
#include <utility>

CollisionSystem::CollisionSystem(Registry_T& registry) : System{registry, 0} {
    m_cursorMoveHandle = EventDispatcher::onCursorMove.subscribe([this](const CursorEvent& e) {
        this->m_registry.view<PlayerComponent, TransformationComponent, CameraComponent>().each(
            [&](PlayerComponent& player, TransformationComponent& transform, CameraComponent& camera) {
                this->updatePlayerLookAtBlock(player, transform, camera);
            });
    });
}

void CollisionSystem::_update(int dt) {
    auto view = m_registry.view<TransformationComponent, CollisionComponent, VelocityComponent>();

    for (auto it = view.begin(); it != view.end(); it++) {
        for (auto jt = it; jt != view.end(); jt++) {
            if (it == jt)
                continue;

            checkCollisions(*it, *jt);
        }
    }

    for (auto it = view.begin(); it != view.end(); it++) {
        checkBlockCollisions(*it);
    }
}

void CollisionSystem::updatePlayerLookAtBlock(PlayerComponent& player, TransformationComponent& transform,
                                              CameraComponent& camera) const {
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

    for (std::vector<glm::vec3>::iterator it = lookAtPositions.begin(); it != lookAtPositions.end(); it++) {
        Block block = World::getBlock(&m_registry, *it);

        if (block.isSolid()) {
            player.lookAt = *it;

            break;
        }
    }
}

void CollisionSystem::checkCollisions(entt::entity first, entt::entity secnd) {
    const TransformationComponent& firstTransform = m_registry.get<TransformationComponent>(first);
    const CollisionComponent& firstCollision = m_registry.get<CollisionComponent>(first);

    const TransformationComponent& secndTransform = m_registry.get<TransformationComponent>(secnd);
    const CollisionComponent& secndCollision = m_registry.get<CollisionComponent>(secnd);

    Cuboid c1 = firstCollision.transform(firstTransform);
    Cuboid c2 = secndCollision.transform(secndTransform);

    bool intersection = c1.intersects(c2);

    if (intersection) {
        // TODO: handle collision

        return;
    }
}

void CollisionSystem::checkBlockCollisions(entt::entity entity) {
    CollisionComponent& collision = m_registry.get<CollisionComponent>(entity);
    TransformationComponent& transform = m_registry.get<TransformationComponent>(entity);

    Cuboid hitbox = collision.transform(transform);
    glm::vec3 minBlock = glm::floor(hitbox.min);
    glm::vec3 maxBlock = glm::floor(hitbox.max);

    bool hasCollision = false;
    glm::vec3 block;
    glm::vec3 offset = glm::vec3(-0.5, -0.5, -0.5);

    for (int x = minBlock.x; x <= maxBlock.x; x++) {
        for (int y = minBlock.y; y <= maxBlock.y; y++) {
            for (int z = minBlock.z; z <= maxBlock.z; z++) {
                glm::vec3 position = glm::vec3(x, y, z);

                if (World::getBlock(&m_registry, position).isSolid()) {
                    Cuboid blockHitbox = Cuboid(position + offset, 1, 1, 1);

                    hasCollision = hitbox.intersects(blockHitbox);
                    if (hasCollision) {
                        block = position;
                    }
                }
            }
        }
    }

    if (hasCollision) {
        glm::vec3 mtv = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        VelocityComponent& velocity = m_registry.get<VelocityComponent>(entity);

        for (int i = 0; i < 6; i++) {
            glm::vec3 blockNormal = glm::vec3();
            blockNormal[i / 2] = i % 2 ? -1 : 1;

            if (!World::getBlock(&m_registry, block + blockNormal).isSolid()) {
                glm::vec3 facePosition = block + 0.5f * blockNormal;

                glm::vec3 v1 = facePosition - hitbox.min;
                glm::vec3 v2 = facePosition - hitbox.max;

                float d1 = glm::dot(v1, blockNormal);
                float d2 = glm::dot(v2, blockNormal);

                glm::vec3 v = glm::max(d1, d2) * blockNormal;

                if (glm::length(v) < glm::length(mtv)) {
					mtv = v;
                }
            }
        }

        transform.move(mtv);

        // set velocity to 0
        velocity.velocity = glm::vec3();

    }	
	
	if (m_registry.any<PlayerComponent>(entity)) {
		PlayerComponent& player = m_registry.get<PlayerComponent>(entity);
        Block b = World::getBlock(&m_registry, transform.getPosition() + glm::vec3(0, -2, 0));

        player.isFalling = !b.isSolid();
        std::cout << player.isFalling << std::endl;
    }
}