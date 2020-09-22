#pragma once

#include <glm/glm.hpp>

struct PointLight {
    glm::vec3 position = glm::vec3(0.0f);

    glm::vec3 ambient = glm::vec3(0.0f);
    glm::vec3 diffuse = glm::vec3(0.0f);
    glm::vec3 specular = glm::vec3(0.0f);

    // attenuation
    float constant = 1.0f, linear = 0.0f, quadratic = 0.0f;
};

struct DirectionalLight {
    glm::vec3 direction = glm::vec3(0, -1, 0);

    glm::vec3 ambient = glm::vec3(0.0f);
    glm::vec3 diffuse = glm::vec3(0.0f);
    glm::vec3 specular = glm::vec3(0.0f);
};
