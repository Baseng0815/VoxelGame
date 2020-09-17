#include "../../include/Systems/PhysicsSystem.h"

#include "../../include/Components/CollisionComponent.h"
#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/TransformationComponent.h"
#include "../../include/Components/VelocityComponent.h"

#include "../../include/Collision.h"
#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"
#include "../../include/World.h"

PhysicsSystem::PhysicsSystem(Registry_T& registry) : System{registry, 0} {
    m_blockCollisionHandle = EventDispatcher::onBlockCollision.subscribe(
        [&](const BlockCollisionEvent& e) { handleBlockCollision(e.entity, e.blockPos); });
}

void PhysicsSystem::_update(int millis) {
    float dt = millis / 1000.f;

    m_registry.view<PlayerComponent, TransformationComponent, VelocityComponent>().each(
        [&](PlayerComponent& player, TransformationComponent& transform, VelocityComponent& velocity) {
            updatePlayer(dt, player, transform, velocity);
        });

    m_registry.view<TransformationComponent, VelocityComponent>(entt::exclude<PlayerComponent>)
        .each([&](TransformationComponent& transform, VelocityComponent& velocity) {
            applyVelocities(dt, transform, velocity);
        });
}

void PhysicsSystem::updatePlayer(float dt, PlayerComponent& player, TransformationComponent& transform,
                                 VelocityComponent& velocity) const {
    if (!World::chunkCreated(Utility::GetChunk(transform.getPosition())))
        return;

    glm::vec3 oldPlayerPos = transform.getPosition();
    glm::ivec2 oldChunk = Utility::GetChunk(oldPlayerPos);

    // check collisions

    if (player.isFalling) {
        float g = -9.8f;

        velocity.velocity.y = glm::min(dt * g + velocity.velocity.y, 10.0f);
    }

    applyVelocities(dt, transform, velocity);

    glm::vec3 newPlayerPos = transform.getPosition();
    glm::ivec2 newChunk = Utility::GetChunk(newPlayerPos);

    if (newChunk != oldChunk) {
        EnterChunkEvent e{nullptr, oldChunk.x, oldChunk.y, newChunk.x, newChunk.y};
        EventDispatcher::raiseEvent(e);
    }

    // update is falling
    // player.isFalling = true;
}

void PhysicsSystem::applyVelocities(float dt, TransformationComponent& transform, VelocityComponent& velocity) {
    transform.move(dt * velocity.velocity);

    float phi = dt * glm::length(velocity.angularVelocity);
    glm::quat pRot = glm::quat(Utility::radToDeg(phi), glm::normalize(velocity.angularVelocity));

    transform.rotate(pRot);
}

void PhysicsSystem::handleBlockCollision(entt::entity entity, glm::vec3 block) const {
    TransformationComponent& transform = m_registry.get<TransformationComponent>(entity);
    VelocityComponent& velocity = m_registry.get<VelocityComponent>(entity);
    CollisionComponent& collision = m_registry.get<CollisionComponent>(entity);

    Math::Cuboid entityCol = collision.transform(transform);
    Math::Cuboid blockCol = Collision::getBlockCollision(block);

    std::vector<glm::vec3> tvs = Collision::getTVs(blockCol, entityCol);

    glm::vec3 mtv;

    auto it = tvs.begin();
    while (it != tvs.end()) {
        glm::vec3 tmp = glm::normalize(*it);

        if (!World::getBlock(&m_registry, block + tmp).isSolid()) {
            mtv = *it;
            transform.move(1.01f * mtv);
            velocity.velocity -= glm::dot(velocity.velocity, mtv) * mtv;
            break;
        }
        it++;
    }
}