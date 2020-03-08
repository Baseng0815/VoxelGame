#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

struct TransformationComponent {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 getModelMatrix() const;
};
