#include "../include/Ray.h"

Ray::Ray() : Ray(glm::vec3(), glm::vec3()) {}

Ray::Ray(glm::vec3 origin, glm::vec3 direction) 
    : origin(origin), direction(glm::normalize(direction)) {}

glm::vec3 Ray::getPoint(float lambda) const {
    return origin + lambda * direction;
}

glm::vec3 Ray::getDirection() const {
    return direction;
}

void Ray::setDirection(const glm::vec3 dir) {
    direction = glm::normalize(dir);
}