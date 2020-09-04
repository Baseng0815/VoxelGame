#include "../../include/Components/CollisionComponent.h"
#include "../../include/Components/TransformationComponent.h"

CollisionComponent::CollisionComponent(glm::vec3 offset, float sizeX, float sizeY, float sizeZ) 
    : localCuboid(offset, sizeX, sizeY, sizeZ) {

    }

Cuboid CollisionComponent::transform(const TransformationComponent& transform) const {
    glm::vec3 position = transform.getPosition();

    return Cuboid(localCuboid.min + position, localCuboid.max + position);
}