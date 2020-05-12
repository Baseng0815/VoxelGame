#pragma once

#include <glm/glm.hpp>

struct UiProperties {
    bool isVisible;
    glm::vec2 position = glm::vec3(0), size = glm::vec3(0);
    glm::vec3 backgroundColor = glm::vec3(0);
};
