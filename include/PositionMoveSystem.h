#pragma once

#include "System.h"

class PositionMoveSystem : public System {
    private:
        void _update(int dt) override;

    public:
        PositionMoveSystem(SystemManager* systemManager);
};
