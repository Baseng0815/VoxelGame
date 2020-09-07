#include "../../include/Systems/CollisionSystem.h"

#include "../../include/Components/CameraComponent.h"
#include "../../include/Components/CollisionComponent.h"
#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/TransformationComponent.h"
#include "../../include/Components/VelocityComponent.h"

#include "../../include/Events/Event.h"
#include "../../include/Events/EventDispatcher.h"

#include "../../include/Math/Plane.h"
#include "../../include/Math/Ray.h"

#include "../../include/Utility.h"
#include "../../include/World.h"

#include <iostream>
#include <utility>

CollisionSystem::CollisionSystem(Registry_T& registry) : System{registry, 0} {
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

    auto playerView = m_registry.view<PlayerComponent, TransformationComponent, CameraComponent>();

    for (auto entity : playerView) {
        PlayerComponent& player = m_registry.get<PlayerComponent>(entity);
        TransformationComponent& transformation = m_registry.get<TransformationComponent>(entity);
        CameraComponent& camera = m_registry.get<CameraComponent>(entity);

        updatePlayerLookAtBlock(player, transformation, camera);
    }
}

void CollisionSystem::updatePlayerLookAtBlock(PlayerComponent& player, TransformationComponent& transform,
                                              CameraComponent& camera) const {
    Ray lookDirection = Ray(transform.getPosition() + camera.playerOffset, camera.front);

    glm::vec3 lookAt =
        lookDirection.getFirstBlock(5, [&](glm::vec3 pos) { return World::getBlock(&m_registry, pos).isSolid(); });

    if (player.lookAt != lookAt) {
        std::cout << "look at: " << lookAt << std::endl;
    }

    player.lookAt = lookAt;
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
            glm::vec3 faceNormal = glm::vec3();
            faceNormal[i / 2] = i % 2 ? -1 : 1;

            if (!World::getBlock(&m_registry, block + faceNormal).isSolid()) {
                glm::vec3 facePosition = getFacePosition(block, faceNormal);

                glm::vec3 v1 = facePosition - hitbox.min;
                glm::vec3 v2 = facePosition - hitbox.max;

                float d1 = glm::dot(v1, faceNormal);
                float d2 = glm::dot(v2, faceNormal);

                glm::vec3 v = glm::max(d1, d2) * faceNormal;

                if (glm::length(v) < glm::length(mtv)) {
                    mtv = v;
                }
            }
        }

        transform.move(mtv);

        velocity.velocity = glm::vec3();
    }

    // current entity is player
    if (m_registry.any<PlayerComponent>(entity)) {
        PlayerComponent& player = m_registry.get<PlayerComponent>(entity);

        // update is falling
        player.isFalling = !World::getBlock(&m_registry, glm::floor(transform.getPosition() + glm::vec3(0, -0.5, 0))).isSolid();
    }
}