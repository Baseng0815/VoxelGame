#pragma once
#include <glm/glm.hpp>
#include "../../include/Cuboid.h"

struct TransformationComponent;

struct CollisionComponent {
    float sizeX, sizeY, sizeZ;
    glm::vec3 offset;

    CollisionComponent(glm::vec3 offset, float sizeX, float sizeY, float sizeZ);

    Cuboid getCuboid(const TransformationComponent& transform) const;
};