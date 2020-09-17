#include "../include/Collision.h"

#include "../include/Components/CollisionComponent.h"
#include "../include/Components/TransformationComponent.h"

#include "../include/Math/Cuboid.h"
#include "../include/Math/Plane.h"
#include "../include/Math/Ray.h"

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

std::vector<glm::vec3> Collision::getTVs(const Math::Cuboid& first, const Math::Cuboid& secnd) {
    std::vector<glm::vec3> tvs = std::vector<glm::vec3>{};

    const std::vector<Math::Plane> firstFaces = first.getFacePlanes();
    const std::vector<glm::vec3> secndCorners = secnd.getCornerPoints();

    for (auto face : firstFaces) {
        const glm::vec3 normal = face.normal;
        glm::vec3 mtv = glm::vec3{FLT_MAX};

        for (auto corner : secndCorners) {
            glm::vec3 p_ = face.getIntersectionPoint(Math::Ray{corner, -normal});
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

            while (it != tvs.end() ? false : glm::length(*it) < glm::length(mtv)) {
                it++;
            }

            tvs.emplace(it, mtv);
        }
    }

    return tvs;
}