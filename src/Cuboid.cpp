#include "../include/Cuboid.h"

Cuboid::Cuboid(glm::vec3 position, glm::vec3 x, glm::vec3 y, glm::vec3 z)
    : position(position), x(x), y(y), z(z) { }

std::vector<glm::vec3> Cuboid::getCornerPoints() const {
    std::vector<glm::vec3> points = {
        position,
        position + x,
        position + y,
        position + z,
        position + x + y,
        position + x + z,
        position + y + z,
        position + x + y + z
    };

    return points;
}