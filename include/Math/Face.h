#include <glm/glm.hpp>
#include <vector>

struct Ray;

struct Face {
    std::vector<glm::vec3> corners;

    Face(std::initializer_list<glm::vec3> corners);

    //bool intersects(const Ray& ray, float* t) const;
};