#include <glm/glm.hpp>
#include <vector>

struct Cuboid {
    glm::vec3 position;
    glm::vec3 x;
    glm::vec3 y;
    glm::vec3 z;

    Cuboid(glm::vec3 position, glm::vec3 x, glm::vec3 y, glm::vec3 z);

    std::vector<glm::vec3> getCornerPoints() const;
};