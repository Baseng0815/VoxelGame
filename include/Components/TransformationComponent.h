#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

struct TransformationComponent {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    TransformationComponent(glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
    glm::mat4 getModelMatrix() const;
};
