#include "../../include/Math/Plane.h"

#include "../../include/Math/Ray.h"
namespace Math {
    Plane::Plane() : Plane(glm::vec3(), glm::vec3()) {}

    Plane::Plane(glm::vec3 normal, glm::vec3 p) {
        this->normal = glm::normalize(normal);
        d = glm::dot(this->normal, p);
    }

    Plane::Plane(glm::vec3 normal, float d) {
        this->normal = glm::normalize(normal);
        this->d = d / glm::length(normal);
    }

    glm::vec3 Plane::getIntersectionPoint(const Ray ray) const {
        if (glm::dot(ray.getDirection(), normal) == 0) {
            return glm::vec3(nanf(""), nanf(""), nanf(""));
        }

        //     ->   ->       ->    ->  ->      ->  -> 
        // d = n * (p  + s * r ) = n * p + s * n * r
        //          ->  ->
        //      d - n * p
        // s = ------------
        //       ->    ->
        //       n  *  r

        float lambda = (d - glm::dot(ray.origin, normal)) / glm::dot(ray.getDirection(), normal);

        return ray.getPoint(lambda);
    }
}