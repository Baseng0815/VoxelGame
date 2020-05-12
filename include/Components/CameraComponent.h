#pragma once

#include "../../include/Rendering/Window.h"
#include "../../include/Configuration.h"

struct CameraComponent {
    glm::mat4 projectionMatrix, viewMatrix;
    glm::vec3 front, front_noY, right;
    int width  = Configuration::getFloatValue("WINDOW_WIDTH");
    int height = Configuration::getFloatValue("WINDOW_HEIGHT");

    glm::vec3 relVelocity = glm::vec3();

    float yaw = 0, pitch = 0, fov;

    bool isValid = false;

    CameraComponent(float fov);
};
