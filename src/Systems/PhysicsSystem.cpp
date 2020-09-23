#include "../../include/Systems/PhysicsSystem.hpp"

#include "../../include/Components/CameraComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/VelocityComponent.hpp"
#include "../../include/Components/CollisionComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"

#include "../../include/Collision.hpp"
#include "../../include/Configuration.hpp"
#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/World.hpp"

void PhysicsSystem::_update(int millis) {
    float dt = millis / 1000.f;

    entt::entity player = m_registry.view<PlayerComponent>().front();
    PlayerComponent &playerComponent = m_registry.get<PlayerComponent>(player);
    TransformationComponent &transformComponent = m_registry.get<TransformationComponent>(player);
    VelocityComponent &velocityComponent = m_registry.get<VelocityComponent>(player);
    CameraComponent &cameraComponent = m_registry.get<CameraComponent>(player);

    updatePlayer(dt, playerComponent, transformComponent, velocityComponent, cameraComponent);

    // looks like velocities are already applied inside updatePlayer?
    /* applyVelocities(dt, transform, velocity); */
}

void PhysicsSystem::updatePlayer(float dt, PlayerComponent& player, TransformationComponent& transform,
                                 VelocityComponent& velocity, CameraComponent &cameraComponent) const {
    if (!World::chunkCreated(Utility::GetChunk(transform.getPosition())))
        return;

    glm::vec3 oldPlayerPos = transform.getPosition();
    glm::ivec2 oldChunk = Utility::GetChunk(oldPlayerPos);

    // check collisions

    if (player.isFalling) {
        float g = -9.8f;

        velocity.velocity.y = glm::min(dt * g + velocity.velocity.y, 10.0f);
    }

    if (applyVelocities(dt, transform, velocity)) {
        cameraComponent.viewMatrixOutdated = true;
    }

    const glm::vec3 &newPlayerPos = transform.getPosition();
    const glm::ivec2 &newChunk = Utility::GetChunk(newPlayerPos);

    if (newChunk != oldChunk) {
        EnterChunkEvent e {nullptr, oldChunk.x, oldChunk.y, newChunk.x, newChunk.y};
        EventDispatcher::raiseEvent(e);
    }

    // update is falling
    if (!World::getBlock(&m_registry, transform.getPosition() - glm::vec3{0, 1, 0}).isSolid()) {
        player.isFalling = true;
    }
}

bool PhysicsSystem::applyVelocities(float dt, TransformationComponent& transform, const VelocityComponent &velocity) {
    bool wasMoved = false;

    if (velocity.velocity != glm::vec3 {0.f}) {
        transform.move(dt * velocity.velocity);
        wasMoved = true;
    }

    float phi = dt * glm::length(velocity.angularVelocity);
    glm::quat pRot = glm::quat(Utility::radToDeg(phi), glm::normalize(velocity.angularVelocity));

    transform.rotate(pRot);

    return wasMoved;
}

void PhysicsSystem::handleBlockCollision(entt::entity entity, const glm::vec3 &block) const {
    TransformationComponent& transform = m_registry.get<TransformationComponent>(entity);
    VelocityComponent& velocity = m_registry.get<VelocityComponent>(entity);
    CollisionComponent& collision = m_registry.get<CollisionComponent>(entity);

    Math::Cuboid entityCol = collision.transform(transform);

    std::vector<glm::vec3> tvs = Collision::getBlockTVs(block, entityCol);

    if (tvs.size() == 0)
        return;

    glm::vec3 mtv;
    auto it = tvs.begin();
    do {
        glm::vec3 normal = glm::normalize(*it);

        if (!World::getBlock(&m_registry, block + normal).isSolid()) {
            mtv = *it;

            transform.move(mtv);

            velocity.velocity -= glm::dot(velocity.velocity, normal) * normal;

            break;
        }

    } while (++it != tvs.end());

    if (m_registry.any<PlayerComponent>(entity)) {
        PlayerComponent& player = m_registry.get<PlayerComponent>(entity);

        if (velocity.velocity.y == 0) {
            player.isFalling = false;
        }
    }
}

PhysicsSystem::PhysicsSystem(Registry_T& registry) : System{registry, 0}
{
    m_blockCollisionHandle = EventDispatcher::onBlockCollision.subscribe(
        [&](const BlockCollisionEvent &e) { handleBlockCollision(e.entity, e.block); });
}
