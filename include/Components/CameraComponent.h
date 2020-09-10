#pragma once

#include "../../include/Rendering/Window.h"
#include "../../include/Configuration.h"

struct CameraComponent {
    glm::mat4 perspectiveProjection, viewMatrix;
    glm::vec3 front, front_noY, right;
    float width;
    float height;

    /* glm::vec3 playerOffset = glm::vec3(0, 0, 1.5f); */
    glm::vec3 playerOffset = glm::vec3 {0.f, 1.5f, 0.f};

    float yaw = -90.f, pitch = 0.f, fov;

    CameraComponent(float fov, float width, float height);
};
