#pragma once

#include "../../include/Rendering/Window.hpp"
#include "../../include/Configuration.hpp"

struct TransformationComponent;

struct CameraComponent {
    bool viewMatrixOutdated = true;

    glm::mat4 perspectiveProjection, viewMatrix;
    // TODO front is initialized so glm::normalize doesn't return nan
    // find better fix maybe some time later
    glm::vec3 front {1.f}, front_noY, right;
    float width;
    float height;

    glm::vec3 playerOffset = glm::vec3 {0.f, 1.5f, 0.f};

    float yaw = -90.f, pitch = 0.f, fov;

    CameraComponent(float fov, float width, float height);

    void invertPitch();

    void updateMatrices(const TransformationComponent &playerTransform);
    void updateViewMatrix(const TransformationComponent &playerTransform);
    void updateProjectionMatrix();
};
