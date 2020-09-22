#pragma once

#include <glm/glm.hpp>

struct VelocityComponent {
    glm::vec3 velocity = glm::vec3();
    glm::vec3 angularVelocity = glm::vec3();
};
