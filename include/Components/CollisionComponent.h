#pragma once
#include <glm/glm.hpp>

#include "../../include/Math/Cuboid.h"

struct TransformationComponent;

struct CollisionComponent {
    Math::Cuboid localCuboid;

    CollisionComponent(glm::vec3 offset, float sizeX, float sizeY, float sizeZ);

    Math::Cuboid transform(const TransformationComponent& transform) const;
};