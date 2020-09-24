#include "../../include/Math/Ray.hpp"
#include "../../include/Math/Plane.hpp"

namespace Math {
Ray::Ray()
    : Ray(glm::vec3(), glm::vec3()) {
}

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
    : origin(origin), direction(glm::normalize(direction)) {
}

glm::vec3 Ray::getPoint(float t) const {
    return origin + t * direction;
}

glm::vec3 Ray::getDirection() const {
    return direction;
}

void Ray::setDirection(const glm::vec3 dir) {
    direction = glm::normalize(dir);
}

bool Ray::intersects(const Plane &plane, float *t) const {
    *t = 0;

    // ray parallel to plane
    if (glm::dot(direction, plane.normal) == 0) {
        return (glm::dot(origin, plane.normal) == plane.d);
    }

    *t = (plane.d - glm::dot(plane.normal, origin)) / (glm::dot(plane.normal, direction));

    return true;
}

glm::vec3 Ray::getFirstBlock(float maxT, std::function<bool(glm::vec3)> predicate) const {
    float t = 0;
    glm::vec3 position;

    float stepX = glm::abs(1 / direction.x);
    float stepY = glm::abs(1 / direction.y);
    float stepZ = glm::abs(1 / direction.z);

    float tx = 0;
    float ty = 0;
    float tz = 0;

    do {
        float tmin = glm::min(tx, glm::min(ty, tz));

        if (tmin == tx) {
            t = tx;
            tx += stepX;
        }

        if (tmin == ty) {
            t = ty;
            ty += stepY;
        }

        if (tmin == tz) {
            t = tz;
            tz += stepZ;
        }

        position = glm::floor(getPoint(t));
    } while (glm::abs(t) <= maxT && !predicate(position));

    if (t > maxT) {
        return glm::vec3(0, -1, 0);
    }
    else {
        return position;
    }
}
} // namespace Math
