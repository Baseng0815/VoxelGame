#include "../../include/Math/Cuboid.h"
#include "../../include/Math/Plane.h"
#include "../../include/Math/Ray.h"

namespace Math {
    Cuboid::Cuboid() : min(glm::vec3()), max(glm::vec3()) {
    }

    Cuboid::Cuboid(glm::vec3 min, glm::vec3 max) : min(min), max(max) {
    }

    Cuboid::Cuboid(glm::vec3 position, float sizeX, float sizeY, float sizeZ) {
        min = max = position;

        if (sizeX < 0.0f)
            min.x += sizeX;
        else
            max.x += sizeX;

        if (sizeY < 0.0f)
            min.y += sizeY;
        else
            max.y += sizeY;

        if (sizeZ < 0.0f)
            min.z += sizeZ;
        else
            max.z += sizeZ;
    }

    std::vector<glm::vec3> Cuboid::getCornerPoints() const {
        glm::vec3 x = glm::vec3(max.x - min.x, 0, 0);
        glm::vec3 y = glm::vec3(0, max.y - min.y, 0);
        glm::vec3 z = glm::vec3(0, 0, max.z - min.z);

        return std::vector<glm::vec3>{min, min + x, min + y, min + z,
            min + x + y, min + x + z, min + y + z, min + x + y + z};
    }

    std::vector<Plane> Cuboid::getFacePlanes() const {
        std::vector<Plane> planes = std::vector<Plane>();

        // push_back kopiert die Elemente; wenn das unnoetig ist, ist emplace_back besser
        planes.emplace_back(Plane{ glm::vec3 {-1, 0, 0}, min });
        planes.emplace_back(Plane{ glm::vec3 {0, -1, 0}, min });
        planes.emplace_back(Plane{ glm::vec3 {0, 0, -1}, min });

        planes.emplace_back(Plane{ glm::vec3 {1, 0, 0}, max });
        planes.emplace_back(Plane{ glm::vec3 {0, 1, 0}, max });
        planes.emplace_back(Plane{ glm::vec3 {0, 0, 1}, max });

        return planes;
    }

    bool Cuboid::intersects(const Cuboid& other) const {
        return other.min.x < max.x&& other.min.y < max.y&& other.min.z < max.z&& min.x < other.max.x&&
            min.y < other.max.y&& min.z < other.max.z;
    }

    bool Cuboid::intersects(const Ray& ray, float* t) const {
        bool intersects = false;
        *t = FLT_MAX;

        std::vector<Plane> facePlanes = getFacePlanes();
        for (Plane face : facePlanes) {
            float tmp = 0;
            if (ray.intersects(face, &tmp)) {
                intersects = true;
                *t = glm::min(*t, tmp);
            }
        }

        return intersects;
    }

    glm::vec3 Cuboid::getPosition() const {
        return min;
    }

    void Cuboid::getSize(float* sizeX, float* sizeY, float* sizeZ) const {
        glm::vec3 size = max - min;

        *sizeX = size.x;
        *sizeY = size.y;
        *sizeZ = size.z;
    }

    Cuboid Cuboid::merge(const Cuboid c1, const Cuboid c2) {
        float minX, minY, minZ, maxX, maxY, maxZ;

        minX = (c1.min.x < c2.min.x) ? c1.min.x : c2.min.x;
        minY = (c1.min.y < c2.min.y) ? c1.min.y : c2.min.y;
        minZ = (c1.min.z < c2.min.z) ? c1.min.z : c2.min.z;

        maxX = (c1.max.x > c2.max.x) ? c1.max.x : c2.max.x;
        maxY = (c1.max.y > c2.max.y) ? c1.max.y : c2.max.y;
        maxZ = (c1.max.z > c2.max.z) ? c1.max.z : c2.max.z;

        return Cuboid(glm::vec3(minX, minY, minZ), glm::vec3(maxX, maxY, maxZ));
    }
}
