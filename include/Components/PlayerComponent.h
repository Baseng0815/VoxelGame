#pragma once
#include <glm/glm.hpp>

struct PlayerComponent {
    // 30 m / s
    float maxMovementSpeed = 5.0f;

    float xAxisInput, yAxisInput, zAxisInput;

    bool isFalling = false;

    glm::vec3 lookAt;
};
