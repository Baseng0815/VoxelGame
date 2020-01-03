#pragma once

#include <glm/glm.hpp>

struct Light {
    glm::vec3 position;
    glm::vec3 color;

    Light(glm::vec3 position = glm::vec3(0, 5, 0), glm::vec3 color = glm::vec3(1, 1, 1));
};
