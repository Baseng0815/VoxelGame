#pragma once

#include "../Window.h"
#include "../Configuration.h"

struct CameraComponent {
    glm::mat4 perspectiveProjection, orthoProjection, viewMatrix;
    glm::vec3 front, front_noY, right;
    float width  = Configuration::INITIAL_WINDOW_WIDTH;
    float height = Configuration::INITIAL_WINDOW_HEIGHT;

    glm::vec3 relVelocity = glm::vec3(0);

    float yaw = 0, pitch = 0, fov;
    bool inputEnabled = true;

    CameraComponent(float fov);
};
