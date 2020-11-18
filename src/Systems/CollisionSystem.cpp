#include "../../include/Systems/CollisionSystem.hpp"

#include "../../include/Components/CameraComponent.hpp"
#include "../../include/Components/CollisionComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/RigidBodyComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"
#include "../../include/Components/VelocityComponent.hpp"

#include "../../include/Events/EventDispatcher.hpp"

#include "../../include/Math/Plane.hpp"
#include "../../include/Math/Ray.hpp"

#include "../../include/World.hpp"

CollisionSystem::CollisionSystem(Registry_T &registry)
    : System{registry, 0} {
}

void CollisionSystem::_update(int dt) {
    auto view = m_registry.view<TransformationComponent, CollisionComponent, VelocityComponent>();

    // for (auto it = view.begin(); it != view.end(); it++) {
    //     for (auto jt = it; jt != view.end(); jt++) {
    //         if (it == jt)
    //             continue;

    //         checkCollisions(*it, *jt);
    //     }
    // }

    for (auto it = view.begin(); it != view.end(); it++) {
        // to avoid tunneling throght terrain at begin wait for terrain generation
        checkBlockCollisions(*it);
    }

    auto playerView = m_registry.view<PlayerComponent, TransformationComponent, CameraComponent>();

    for (auto entity : playerView) {
        PlayerComponent &player = m_registry.get<PlayerComponent>(entity);
        TransformationComponent &transformation = m_registry.get<TransformationComponent>(entity);
        CameraComponent &camera = m_registry.get<CameraComponent>(entity);

        updatePlayerLookAtBlock(player, transformation, camera);
    }
}

void CollisionSystem::updatePlayerLookAtBlock(PlayerComponent &player, TransformationComponent &transform, CameraComponent &camera) const {
    Math::Ray lookDirection = Math::Ray(transform.getPosition() + camera.positionOffset, camera.front);

    glm::vec3 lookAt = lookDirection.getFirstBlock(5, [&](glm::vec3 pos) {
        return World::getBlock(m_registry, pos).isSolid();
    });

    player.lookAt = lookAt;
}

void CollisionSystem::checkCollisions(entt::entity first, entt::entity secnd) {
    const TransformationComponent &firstTransform = m_registry.get<TransformationComponent>(first);
    const CollisionComponent &firstCollision = m_registry.get<CollisionComponent>(first);

    const TransformationComponent &secndTransform = m_registry.get<TransformationComponent>(secnd);
    const CollisionComponent &secndCollision = m_registry.get<CollisionComponent>(secnd);

    Math::Cuboid c1 = firstCollision.transform(firstTransform);
    Math::Cuboid c2 = secndCollision.transform(secndTransform);

    bool intersection = c1.intersects(c2);

    if (intersection) {
        // TODO: handle collision

        return;
    }
}

void CollisionSystem::checkBlockCollisions(entt::entity entity) {
    CollisionComponent &collision = m_registry.get<CollisionComponent>(entity);
    TransformationComponent &transform = m_registry.get<TransformationComponent>(entity);
    VelocityComponent &velocity = m_registry.get<VelocityComponent>(entity);

    glm::vec3 currPos = transform.getPosition();
    if (!World::chunkCreated(Utility::getChunk(currPos))) {
        return;
    }

    bool hasCollision = false;
    glm::vec3 block;

    Math::Cuboid hitbox = collision.transform(currPos);
    glm::vec3 minBlock = glm::floor(hitbox.min);
    glm::vec3 maxBlock = glm::floor(hitbox.max);

    for (int x = minBlock.x; x <= maxBlock.x; x++) {
        for (int y = minBlock.y; y <= maxBlock.y; y++) {
            for (int z = minBlock.z; z <= maxBlock.z; z++) {
                glm::vec3 position = glm::vec3(x, y, z);

                if (World::getBlock(m_registry, position).isSolid()) {
                    Math::Cuboid blockHitbox = Math::Cuboid(position, 1, 1, 1);

                    if (hitbox.intersects(blockHitbox)) {
                        hasCollision = true;
                        block = position;
                        break;
                    }
                }
            }
        }
    }

    if (hasCollision) {
        BlockCollisionEvent e{nullptr, entity, block};
        EventDispatcher::raiseEvent(e);
    }
}
