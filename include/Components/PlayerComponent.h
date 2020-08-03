#pragma once

struct PlayerComponent {
    // 30 m / s
    float maxMovementSpeed = 30.0f;

    float xAxisInput, yAxisInput, zAxisInput;

    bool isFalling = false;

    glm::vec3 lookAt;
};
