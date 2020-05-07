#pragma once

#include <glm/glm.hpp>

struct Light {
    glm::vec3 dir;
    glm::vec3 color;

    Light(glm::vec3 direction = glm::vec3(0.5f, 1, 0.5f), glm::vec3 color = glm::vec3(0.8f, 0.8f, 0.8f));
};
