#include "../../include/Systems/PhysicsSystem.hpp"

#include "../../include/Components/CameraComponent.hpp"
#include "../../include/Components/CollisionComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/RigidBodyComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"
#include "../../include/Components/VelocityComponent.hpp"

#include "../../include/Collision.hpp"
#include "../../include/Configuration.hpp"
#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/World.hpp"

void PhysicsSystem::_update(int millis) {
    float dt = millis / 1000.f;

    m_registry.view<TransformationComponent, VelocityComponent, RigidBodyComponent>().each(
        [&](entt::entity entity, TransformationComponent &transform, VelocityComponent &velocity, RigidBodyComponent& rigidBody) {
            if (World::chunkCreated(Utility::GetChunk(transform.getPosition()))) {
                applyAccelerations(dt, velocity, rigidBody);

                applyVelocities(dt, entity, transform, velocity);

                updateFalling(rigidBody, transform, velocity);
            }
        });
}

void PhysicsSystem::applyVelocities(float dt, const entt::entity &entity, TransformationComponent &transform, const VelocityComponent &velocity) const {
    bool wasMoved = false;

    if (velocity.velocity != glm::vec3{0.f}) {
        glm::vec3 oldPos = transform.getPosition();

        transform.move(dt * velocity.velocity);

        EntityMovedEvent e{nullptr, entity, transform.getPosition(), oldPos};
        EventDispatcher::raiseEvent(e);        
    }

    if (velocity.angularVelocity != glm::vec3{0.f}) {
        float phi = dt * glm::length(velocity.angularVelocity);
        glm::quat pRot = glm::quat(Utility::radToDeg(phi), glm::normalize(velocity.angularVelocity));

        transform.rotate(pRot);
    }
}

void PhysicsSystem::applyAccelerations(float dt, VelocityComponent &velocity, const RigidBodyComponent &rigidBody) {
    if (rigidBody.isFalling) {
        velocity.velocity.y = std::max(velocity.velocity.y - dt * 9.81f, -20.f);
    }
}

void PhysicsSystem::handleBlockCollision(entt::entity entity, const glm::vec3 &block) const {
    TransformationComponent &transform = m_registry.get<TransformationComponent>(entity);
    VelocityComponent &velocity = m_registry.get<VelocityComponent>(entity);
    CollisionComponent &collision = m_registry.get<CollisionComponent>(entity);
    RigidBodyComponent &rigidBody = m_registry.get<RigidBodyComponent>(entity);

    Math::Cuboid entityCol = collision.transform(transform);
    Math::Cuboid blockCol = Collision::getBlockCollision(block);

    std::vector<glm::vec3> tvs = Collision::getTVs(blockCol, entityCol);

    if (tvs.size() == 0)
        return;

    glm::vec3 mtv;
    auto it = tvs.begin();
    do {
        glm::vec3 normal = glm::round(glm::normalize(*it));

        if (!World::getBlock(&m_registry, block + normal).isSolid()) {
            mtv = *it;

            transform.move(mtv);

            velocity.velocity -= glm::dot(velocity.velocity, normal) * normal;
            if (velocity.velocity.y == 0) {
                rigidBody.isFalling = false;
            }
            break;
        }

    } while (++it != tvs.end());
}

void PhysicsSystem::updateFalling(RigidBodyComponent &rigidBody, const TransformationComponent &transform, const VelocityComponent &velocity) const {
    const glm::vec3 &position = transform.getPosition();

    if (!World::getBlock(&m_registry, glm::floor(position - glm::vec3{0.0f, 0.9f, 0.0f})).isSolid()) {
        rigidBody.isFalling = true;
    }
    else if(velocity.velocity.y == 0) {
        rigidBody.isFalling = false;
    }
}

PhysicsSystem::PhysicsSystem(Registry_T &registry)
    : System{registry, 0} {
    m_blockCollisionHandle = EventDispatcher::onBlockCollision.subscribe(
        [&](const BlockCollisionEvent &e) { handleBlockCollision(e.entity, e.block); });
}
