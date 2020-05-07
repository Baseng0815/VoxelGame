#pragma once

#include "System.h"

#include <vector>
#include <future>
#include <map>

struct ChunkComponent;

class PhysicsSystem : public System {
private:
	std::vector<entt::entity> movedObjects = std::vector<entt::entity>();

	void _update(int dt) override;

	void solveBlockCollisions();
public:
	PhysicsSystem(SystemManager* systemManager);

	~PhysicsSystem();
};

