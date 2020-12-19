#include "../../include/Components/CollisionComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"

CollisionComponent::CollisionComponent(glm::vec3 offset, float sizeX, float sizeY, float sizeZ) {
    localCuboid = Math::Cuboid{offset, sizeX, sizeY, sizeZ};
}

Math::Cuboid CollisionComponent::transform(const TransformationComponent& transform) const {
    glm::vec3 position = transform.getPosition();
    glm::vec3 scale = transform.getScale();

    glm::vec3 size = localCuboid.max - localCuboid.min;
    glm::vec3 newSize = glm::vec3 {size.x * scale.x, size.y * scale.y, size.z * scale.z};

    return Math::Cuboid(localCuboid.min + position, localCuboid.min + position + newSize);
}
