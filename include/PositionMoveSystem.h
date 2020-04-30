#pragma once

#include "System.h"

class PositionMoveSystem : public System {
    private:

    public:
        PositionMoveSystem(SystemManager* systemManager, SharedContext* context);

        void update(int dt) override;
};
