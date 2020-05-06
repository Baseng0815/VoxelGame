#pragma once

#include "System.h"

#include <vector>
#include <future>
#include <map>

struct RigidBodyComponent;
struct TransformationComponent;

class PhysicSystem : public System {
private:
	void detectCollision(entt::entity, RigidBodyComponent&, TransformationComponent&);

	std::vector<std::pair<entt::entity, entt::entity>> collisions;

public:
	PhysicSystem(SystemManager* systemManager, SharedContext* context);

	void update(int dt) override;

	~PhysicSystem();
};

