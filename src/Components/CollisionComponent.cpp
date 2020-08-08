#include "../../include/Components/CollisionComponent.h"
#include "../../include/Components/TransformationComponent.h"

CollisionComponent::CollisionComponent(glm::vec3 offset, float sizeX, float sizeY, float sizeZ) 
    : offset(offset), sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {}

Cuboid CollisionComponent::getCuboid(const TransformationComponent& transform) const {
    TransformationComponent scaleRotation = TransformationComponent(transform);
    scaleRotation.setPosition(glm::vec3(0, 0, 0));
    glm::mat4 scaleRotationMat = scaleRotation.getModelMatrix();

    glm::vec3 position = transform.getPosition() + offset;
    glm::vec3 x = scaleRotationMat * glm::vec4(sizeX, 0, 0, 0);
    glm::vec3 y = scaleRotationMat * glm::vec4(0, sizeY, 0, 0);
    glm::vec3 z = scaleRotationMat * glm::vec4(0, 0, sizeZ, 0);

    return Cuboid(position, x, y, z);
}