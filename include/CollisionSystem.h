#pragma once

#include "System.h"

#include <vector>
#include <future>
#include <map>

struct ChunkComponent;

class CollisionSystem : public System {
public:
	CollisionSystem(SystemManager* systemManager, SharedContext* context);

	void init() override;
	void update(int dt) override;

	~CollisionSystem();
};

