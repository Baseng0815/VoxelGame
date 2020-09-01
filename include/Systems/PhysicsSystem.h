#pragma once

#include "System.h"

#include <vector>
#include <future>
#include <map>

#include <glm/glm.hpp>

struct BoxCollision;
struct ChunkComponent;
struct TransformationComponent;

class PhysicsSystem : public System {
    private:
        std::vector<entt::entity> movedObjects;

        void _update(int dt) override final;

        void solveBlockCollisions();

        void checkAndHandleCollisions(const BoxCollision&, const BoxCollision&,
                TransformationComponent&, glm::vec3&) const;
    public:
        PhysicsSystem(Registry_T &registry);

        ~PhysicsSystem();
};
