#pragma once
#include <glm/glm.hpp>

struct Ray;

struct Plane {
    glm::vec3 normal;
    glm::vec3 p;
    float d;

    Plane();
    Plane(glm::vec3 normal, glm::vec3 p);

    glm::vec3 getIntersectionPoint(const Ray ray) const;
};