#include "../../include/Systems/CollisionSystem.h"

#include "../../include/Components/CameraComponent.h"
#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/TransformationComponent.h"

#include "../../include/Ray.h"

CollisionSystem::CollisionSystem(entt::registry *registry)
    : System(registry, 0) {
}

void CollisionSystem::_update(int dt) {
    m_registry->view<PlayerComponent, TransformationComponent, CameraComponent>().each(
        [&](PlayerComponent& player, TransformationComponent& transform, CameraComponent& camera) {
            updatePlayerLookAtBlock(player, transform, camera);
        }
    );
}

void CollisionSystem::updatePlayerLookAtBlock(PlayerComponent& player, TransformationComponent& transform, CameraComponent& camera) {
    Ray r = Ray(transform.getPosition() + camera.playerOffset, camera.front);

    glm::vec3 direction = r.getDirection();
    int startX = (direction.x >= 0 ? floor(r.origin.x) : ceil(r.origin.x));
    int startY = (direction.y >= 0 ? floor(r.origin.y) : ceil(r.origin.y));
    int startZ = (direction.z >= 0 ? floor(r.origin.z) : ceil(r.origin.z));

    int stepX = (int)(direction.x / abs(direction.x));
    int stepY = (int)(direction.y / abs(direction.y));
    int stepZ = (int)(direction.z / abs(direction.z));
}