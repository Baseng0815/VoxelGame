#include "../include/Plane.h"

#include "../include/Ray.h"

Plane::Plane() : Plane(glm::vec3(), glm::vec3()) {}

Plane::Plane(glm::vec3 normal, glm::vec3 p) : normal(normal), p(p) {
    d = glm::dot(normal, p);
}


glm::vec3 Plane::getIntersectionPoint(const Ray ray) const {
    if(glm::dot(ray.getDirection(), normal) == 0) {
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