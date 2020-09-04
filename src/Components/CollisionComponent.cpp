#include "../../include/Components/CollisionComponent.h"
#include "../../include/Components/TransformationComponent.h"

CollisionComponent::CollisionComponent(glm::vec3 offset, float sizeX, float sizeY, float sizeZ) 
    : offset(offset), sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {}

Cuboid CollisionComponent::getCuboid(const TransformationComponent& transform) const {        
    glm::vec3 position = transform.getPosition() + offset;    

    return Cuboid(position, sizeX, sizeY, sizeZ);
}