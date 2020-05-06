#pragma once

#include "System.h"

#include <vector>
#include <future>
#include <map>

struct ChunkComponent;

class PhysicsSystem : public System {
    private:
        void _update(int dt) override;

    public:
        PhysicsSystem(SystemManager* systemManager);

        ~PhysicsSystem();
};

