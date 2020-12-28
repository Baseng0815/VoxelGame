#pragma once

#include "../../include/Rendering/Window.hpp"
#include "../../include/Configuration.hpp"

struct TransformationComponent;

struct CameraComponent {
    bool viewMatrixOutdated = true;
    bool projMatrixOutdated = true;

    glm::mat4 perspectiveProjection, viewMatrix;
    // TODO front is initialized so glm::normalize doesn't return nan
    // find better fix maybe some time later
    glm::vec3 front, front_noY, right;
    float width;
    float height;

    glm::vec3 positionOffset = glm::vec3 {0.f, 1.5f, 0.f};

    // fov = fov_y
    float yaw = -90.f, pitch = 0.f, fov;
    float fov_x = 0;

    CameraComponent(float fov, float width, float height);

    void invertPitch();

    // TODO put all of this stuff into systems
    void updateMatrices(const TransformationComponent &playerTransform);
};
