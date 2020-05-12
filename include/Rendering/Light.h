#pragma once

#include <glm/glm.hpp>

struct Light {
    glm::vec3 dir;
    glm::vec3 color;

    Light(glm::vec3 direction = glm::vec3(-0.25f, 1, -0.25f), glm::vec3 color = glm::vec3(0.2f, 0.2f, 0.2f));
};
