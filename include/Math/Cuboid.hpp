#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace Math {
    struct Ray;
    struct Plane;

    constexpr glm::vec3 cuboidNormals[6] = {
        glm::vec3{-1, 0, 0},
        glm::vec3{0, -1, 0},
        glm::vec3{0, 0, -1},
        glm::vec3{1, 0, 0},
        glm::vec3{0, 1, 0},
        glm::vec3{0, 0, 1}
    };

    struct Cuboid {
        public:
            glm::vec3 min;
            glm::vec3 max;

            Cuboid();
            Cuboid(glm::vec3 min, glm::vec3 max);
            Cuboid(glm::vec3 position, float sizeX, float sizeY, float sizeZ);

            std::vector<glm::vec3> getCornerPoints() const;
            std::vector<Plane> getFacePlanes() const;

            bool intersects(const Cuboid& other) const;
            bool intersects(const Ray& ray, float* t) const;

            glm::vec3 getPosition() const;
            void getSize(float* sizeX, float* sizeY, float* sizeZ) const;

            void move(const glm::vec3 ds);

            const glm::vec3 getMin() const;
            const glm::vec3 getMax() const;

            static Cuboid merge(const Cuboid c1, const Cuboid c2);
    };
}
