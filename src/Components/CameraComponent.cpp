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
