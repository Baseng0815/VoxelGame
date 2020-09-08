#pragma once
#include <glm/glm.hpp>
#include "../../include/Math/Cuboid.h"

struct TransformationComponent;

struct CollisionComponent {
    Cuboid localCuboid;

    CollisionComponent(glm::vec3 offset, float sizeX, float sizeY, float sizeZ);

    Cuboid transform(const TransformationComponent& transform) const;
};