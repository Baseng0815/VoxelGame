#pragma once

#include "System.h"

#include <vector>
#include <future>
#include <map>

struct ChunkComponent;

class PhysicSystem : public System {
public:
	PhysicSystem(SystemManager* systemManager, SharedContext* context);
	
	void update(int dt) override;

	~PhysicSystem();
};

