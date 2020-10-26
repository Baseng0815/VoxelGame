#pragma once

#include "../Color.hpp"

// TODO use color
struct PointLight {
    glm::vec3 position {0.0f};

    Color ambient;
    Color diffuse;
    Color specular;

    // attenuation
    // TODO implement this in fragment shader
    float constant = 1.0f, linear = 0.0f, quadratic = 0.0f;
};

struct DirectionalLight {
    glm::vec3 direction {0, -1, 0};

    Color ambient;
    Color diffuse;
    Color specular;
};
