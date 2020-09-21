#include "../include/Collision.h"

#include "../include/Components/CollisionComponent.h"
#include "../include/Components/TransformationComponent.h"

#include "../include/Math/Plane.h"
#include "../include/Math/Ray.h"

#include "../include/World.h"

#include "../include/GameData/GameData.h"

#include <iostream>

Math::Cuboid Collision::getCollision(entt::registry& registry, entt::entity entity) {
    CollisionComponent& collision = registry.get<CollisionComponent>(entity);
    TransformationComponent& transform = registry.get<TransformationComponent>(entity);

    return collision.transform(transform);
}

bool Collision::checkCollision(entt::registry& registry, entt::entity first, entt::entity second) {
    Math::Cuboid firstCollision = getCollision(registry, first);
    Math::Cuboid secndCollision = getCollision(registry, second);

    return firstCollision.intersects(secndCollision);
}

Math::Cuboid Collision::getBlockCollision(glm::vec3 blockPos) {
    return Math::Cuboid{blockPos, 1, 1, 1};
}

glm::vec3 Collision::getMTV(const Math::Cuboid& first, const Math::Cuboid& secnd) {
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

// TODO: Fix this
std::vector<glm::vec3> Collision::getTVs(const Math::Cuboid& first, const Math::Cuboid& secnd) {
    std::vector<glm::vec3> tvs = std::vector<glm::vec3>{};

    const std::vector<Math::Plane> firstFaces = first.getFacePlanes();
    const std::vector<glm::vec3> secndCorners = secnd.getCornerPoints();

    for (auto face : firstFaces) {
        const glm::vec3 normal = face.normal;
        glm::vec3 mtv = glm::vec3{FLT_MAX};

        for (auto corner : secndCorners) {
            glm::vec3 p_ = face.getIntersectionPoint(Math::Ray{corner, normal});
            glm::vec3 d = corner - p_;
            float h = glm::dot(d, normal);

            glm::vec3 v = h * normal;
            if (glm::length(v) != 0 && glm::length(v) < glm::length(mtv)) {
                mtv = v;
            }
        }

        if (tvs.empty()) {
            tvs.emplace_back(mtv);
        }
        else {
            auto it = tvs.begin();

            while (it != tvs.end() ? glm::length(*it) < glm::length(mtv) : false) {
                it++;
            }

            tvs.emplace(it, mtv);
        }
    }

    return tvs;
}

std::vector<glm::vec3> Collision::getBlockTVs(const glm::vec3 block, const Math::Cuboid& collision) {
    // find corner in block
    glm::vec3 cornerInBlock{-1};

    auto corners = collision.getCornerPoints();
    for (glm::vec3 corner : corners) {
        glm::vec3 cornerBlock = glm::floor(corner);

        if (cornerBlock == block) {
            cornerInBlock = corner;
        }
    }    

    std::vector<glm::vec3> tvs = std::vector<glm::vec3>{};
    if(cornerInBlock.y < 0)
        cornerInBlock = collision.min;

    for (int i = 0; i < 6; i++) {
        glm::vec3 tv = glm::vec3{0};
        tv[i / 2] = block[i / 2] - cornerInBlock[i / 2];
        
        if (i % 2) {
            tv[i / 2]++;
        }       

        if(glm::length(tv) != 0) {
            if(tvs.empty()) {
                tvs.emplace_back(tv);
            }
            else {
                auto it = tvs.begin();

                while(it != tvs.end() ? glm::length(*it) < glm::length(tv) : false) {
                    it++;
                }

                tvs.emplace(it, tv);
            }
        }
    }    

    return tvs;
}

std::vector<Math::Cuboid> Collision::getBlockCollisions(const entt::registry* registry, const Math::Cuboid& blocks) {
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
