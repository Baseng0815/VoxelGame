#pragma once

#include "System.h"

#include <vector>
#include <future>
#include <map>

#include <glm/glm.hpp>

struct ChunkComponent;
struct BoxCollision;

class PhysicsSystem : public System {
private:
	std::vector<entt::entity> movedObjects = std::vector<entt::entity>();

	void _update(int dt) override final;

	void solveBlockCollisions();

	void checkAndHandleCollisions(const BoxCollision&, const BoxCollision&, 
		glm::vec3&, glm::vec3&) const;
public:
	PhysicsSystem(SystemManager* systemManager);

	~PhysicsSystem();
};

