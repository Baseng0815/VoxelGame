#include "../../include/Math/Ray.hpp"
#include "../../include/Math/Cuboid.hpp"
#include "../../include/Math/Plane.hpp"
#include "../../include/Utility.hpp"

#include <iostream>
#include <limits.h>

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

    bool Ray::intersects(const Plane& plane, float &t) const {
        t = 0;

        // ray parallel to plane
        if (glm::dot(direction, plane.normal) == 0) {
            return (glm::dot(origin, plane.normal) == plane.d);
        }

        t = (plane.d - glm::dot(plane.normal, origin)) / (glm::dot(plane.normal, direction));

        return true;
    }

    glm::vec3 Ray::getFirstBlock(float maxT, std::function<bool(glm::vec3)> predicate) const {
        float t = 0;
        glm::vec3 position;

        float stepX = glm::abs(1 / direction.x);
        float stepY = glm::abs(1 / direction.y);
        float stepZ = glm::abs(1 / direction.z);

        int nextX = glm::sign(direction.x) > 0 ? glm::ceil(origin.x) : glm::floor(origin.x);
        int nextY = glm::sign(direction.y) > 0 ? glm::ceil(origin.y) : glm::floor(origin.y);
        int nextZ = glm::sign(direction.z) > 0 ? glm::ceil(origin.z) : glm::floor(origin.z);

        float tx = (nextX - origin.x) / direction.x;
        float ty = (nextY - origin.y) / direction.y;
        float tz = (nextZ - origin.z) / direction.z;

        do {
            float tmin = glm::min(tx, glm::min(ty, tz));
            position = glm::floor(getPoint(tmin + 0.0001f));

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
        } while (glm::abs(t) <= maxT && !predicate(position));

        if (t > maxT) {
            return glm::vec3(0, -1, 0);
        }
        else {
            return position;
        }
    }

    glm::vec3 Ray::getIntersectionFaceNormal(const glm::ivec3 block) const {
        int minIndex = 6;
        float minValue = std::numeric_limits<float>::max();
        glm::vec3 blockCenter = (glm::vec3)block + glm::vec3{0.5f};

        for (int i = 0; i < 6; i++) {
            glm::vec3 normal = cuboidNormals[i];
            float d = glm::dot(blockCenter + 0.5f * normal, normal);

            float t = (d - glm::dot(origin, normal)) / glm::dot(direction, normal);

            if (minValue > t) {
                if (Utility::onBlockFace(block, getPoint(t))) {
                    minValue = t;
                    minIndex = i;
                }
            }
        }

        if (minIndex < 6) {
            return cuboidNormals[minIndex];
        }
        else {
            return glm::vec3{0};
        }
    }
} // namespace Math
