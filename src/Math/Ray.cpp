#include "../../include/Math/Ray.h"
#include "../../include/Math/Plane.h"

namespace Math {
    Ray::Ray() : Ray(glm::vec3(), glm::vec3()) {
    }

    Ray::Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(glm::normalize(direction)) {
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

    bool Ray::intersects(const Plane& plane, float* t) const {
        *t = 0;

        // ray parallel to plane
        if (glm::dot(direction, plane.normal) == 0) {
            return (glm::dot(origin, plane.normal) == plane.d);
        }

        *t = (plane.d - glm::dot(plane.normal, origin)) / (glm::dot(plane.normal, direction));

        return true;
    }
}
