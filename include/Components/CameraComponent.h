#pragma once

#include "../Window.h"
#include "../Definitions.h"

struct CameraComponent {
    glm::mat4 projectionMatrix, viewMatrix;
    glm::vec3 front, front_noY, right;
    int width = Definitions::WINDOW_WIDTH, height = Definitions::WINDOW_HEIGHT;

    glm::vec3 relVelocity;

    float yaw = 0, pitch = 0, fov;

    CameraComponent(float fov);
};
