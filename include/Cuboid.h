#include <glm/glm.hpp>
#include <vector>

struct Cuboid {
    glm::vec3 min;
    glm::vec3 max;

    Cuboid(glm::vec3 min, glm::vec3 max);
    Cuboid(glm::vec3 position, float sizeX, float sizeY, float sizeZ);

    std::vector<glm::vec3>& getCornerPoints() const;    
    bool intersects(const Cuboid& other) const;

    glm::vec3 getPosition() const;
    void getSize(float* sizeX, float* sizeY, float* sizeZ) const;

    static Cuboid merge(const Cuboid c1, const Cuboid c2);
};