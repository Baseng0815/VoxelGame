#include <glm/glm.hpp>
#include <vector>
#include <memory>

struct Cuboid {
    glm::vec3 min;
    glm::vec3 max;

    Cuboid(glm::vec3 position, float sizeX, float sizeY, float sizeZ);

    std::unique_ptr<glm::vec3[]> getCornerPoints() const;    
    bool intersects(const Cuboid& other) const;
};