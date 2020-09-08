#pragma once

#include "../../include/Rendering/Window.h"
#include "../../include/Configuration.h"

struct CameraComponent {
    glm::mat4 perspectiveProjection, viewMatrix;
    glm::vec3 front, front_noY, right;
    float width;
    float height;

    glm::vec3 playerOffset = glm::vec3(0, 1.5f, 0);

    float yaw = 0, pitch = 0, fov;    

    CameraComponent(float fov, float width, float height);
};
