#include "../include/Collision.hpp"

#include "../include/Components/CollisionComponent.hpp"
#include "../include/Components/TransformationComponent.hpp"

#include "../include/Math/Plane.hpp"
#include "../include/Math/Ray.hpp"

#include "../include/World.hpp"

#include "../include/GameData/GameData.hpp"

#include <iostream>

Math::Cuboid Collision::getCollision(entt::registry &registry, entt::entity entity) {
    CollisionComponent &collision = registry.get<CollisionComponent>(entity);
    TransformationComponent &transform = registry.get<TransformationComponent>(entity);

    return collision.transform(transform);
}

bool Collision::checkCollision(entt::registry &registry, entt::entity first, entt::entity second) {
    Math::Cuboid firstCollision = getCollision(registry, first);
    Math::Cuboid secndCollision = getCollision(registry, second);

    return firstCollision.intersects(secndCollision);
}

Math::Cuboid Collision::getBlockCollision(glm::vec3 blockPos) {
    return Math::Cuboid{blockPos, 1, 1, 1};
}

glm::vec3 Collision::getMTV(const Math::Cuboid &first, const Math::Cuboid &secnd) {
    if (!first.intersects(secnd)) {
        return glm::vec3{0.0f};
    }

    glm::vec3 mtv = glm::vec3{FLT_MAX};

    const std::vector<Math::Plane> firstFaces = first.getFacePlanes();
    const std::vector<glm::vec3> secndCorners = secnd.getCornerPoints();

    for (auto face : firstFaces) {
        const glm::vec3 normal = face.normal;

        for (auto corner : secndCorners) {
            glm::vec3 p_ = face.getIntersectionPoint(Math::Ray{corner, -normal});
            glm::vec3 d = corner - p_;
            float h = glm::dot(d, normal);

            glm::vec3 v = h * normal;
            if (glm::length(v) != 0 && glm::length(v) < glm::length(mtv)) {
                mtv = v;
            }
        }
    }

    return mtv;
}

std::vector<glm::vec3> Collision::getTVs(const Math::Cuboid &first, const Math::Cuboid &secnd) {
    std::vector<glm::vec3> tvs = std::vector<glm::vec3>();

    std::array<glm::vec3, 3> basisVectors = {glm::vec3{1, 0, 0}, glm::vec3{0, 1, 0}, glm::vec3{0, 0, 1}};

    glm::vec3 posD = first.max - secnd.min;
    glm::vec3 negD = secnd.max - first.min;

    for (const glm::vec3 &basis : basisVectors) {
        float d1 = glm::dot(basis, posD);
        float d2 = glm::dot(basis, negD);

        glm::vec3 mtv1 = d1 * basis;
        glm::vec3 mtv2 = d2 * -basis;

        glm::vec3 tv = (d1 < d2 ? mtv1 : mtv2);

        if (tvs.empty()) {
            tvs.emplace_back(tv);
        }
        else {
            auto it = tvs.begin();
            bool inserted = false;

            do {
                if (glm::length(tv) < glm::length(*it)) {
                    tvs.emplace(it, tv);
                    inserted = true;
                    break;
                }
                else {
                    it++;
                }
            } while (it != tvs.end());

            if (!inserted) {
                tvs.emplace_back(tv);
            }
        }
    }

    return tvs;
}

std::vector<Math::Cuboid> Collision::getBlockCollisions(const entt::registry *registry, const Math::Cuboid &blocks) {
    std::vector<Math::Cuboid> collisions = std::vector<Math::Cuboid>{};

    int minX = std::floor(blocks.getMin().x);
    int minY = std::floor(blocks.getMin().y);
    int minZ = std::floor(blocks.getMin().z);

    int maxX = std::floor(blocks.getMax().x);
    int maxY = std::floor(blocks.getMax().y);
    int maxZ = std::floor(blocks.getMax().z);

    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            for (int z = minZ; z <= maxZ; z++) {
                glm::vec3 block = glm::vec3{x, y, z};
                if (GameData::getBlockTemplate(World::getBlock(registry, block).type).isSolid) {
                    collisions.emplace_back(getBlockCollision(block));
                }
            }
        }
    }

    return collisions;
}
