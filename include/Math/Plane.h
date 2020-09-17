#include <glm/glm.hpp>

namespace Math {
    struct Ray;

    struct Plane {
        glm::vec3 normal;
        float d;

        Plane();
        Plane(glm::vec3 normal, glm::vec3 p);
        Plane(glm::vec3 normal, float d);

        glm::vec3 getIntersectionPoint(const Ray ray) const;
    };
}