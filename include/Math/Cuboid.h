#include <glm/glm.hpp>
#include <vector>

struct Ray;
struct Plane;

struct Cuboid {
    glm::vec3 min;
    glm::vec3 max;

    Cuboid();
    Cuboid(glm::vec3 min, glm::vec3 max);
    Cuboid(glm::vec3 position, float sizeX, float sizeY, float sizeZ);

    // Referenzen auf lokale Variablen nicht gut
    std::vector<glm::vec3> getCornerPoints() const;
    std::vector<Plane> getFacePlanes() const;

    bool intersects(const Cuboid& other) const;
    bool intersects(const Ray& ray, float* t) const;

    glm::vec3 getPosition() const;
    void getSize(float* sizeX, float* sizeY, float* sizeZ) const;

    static Cuboid merge(const Cuboid c1, const Cuboid c2);
};
