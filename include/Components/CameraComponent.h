#pragma once

#include "../../include/Rendering/Window.h"
#include "../../include/Configuration.h"

struct CameraComponent {
    glm::mat4 perspectiveProjection, viewMatrix;
    glm::vec3 front, front_noY, right;
    float width;
    float height;

    glm::vec3 relVelocity = glm::vec3(0);

    float yaw = 0, pitch = 0, fov;

    bool isFalling = false;
    bool isFlying = false;

    unsigned int worldId = 0;

    CameraComponent(float fov, float width, float height);
};
