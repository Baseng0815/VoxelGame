#include "../../include/Components/CameraComponent.hpp"

#include "../../include/Components/TransformationComponent.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>

CameraComponent::CameraComponent(float fov, float width, float height)
    : fov(fov), width(width), height(height) {}

void CameraComponent::invertPitch() {
    pitch *= -1;
    viewMatrixOutdated = true;
}

void CameraComponent::updateMatrices(const TransformationComponent &playerTransform) {
    updateViewMatrix(playerTransform);
    updateProjectionMatrix();
}

void CameraComponent::updateViewMatrix(const TransformationComponent &playerTransform) {
    viewMatrix = glm::lookAt(playerTransform.getPosition() + playerOffset, playerTransform.getPosition() + playerOffset + front, glm::vec3{0, 1, 0});
    viewMatrixOutdated = false;
}

void CameraComponent::updateProjectionMatrix() {
    perspectiveProjection = glm::perspective(glm::radians(fov), width / (float)height, 0.1f, 7000.f);    
}
