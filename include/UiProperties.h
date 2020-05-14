#pragma once

#include <glm/glm.hpp>

struct UiProperties {
    bool isVisible;
    glm::vec2 position = glm::vec3(0), size = glm::vec3(1);
    glm::vec4 backgroundColor = glm::vec4(0);
};
