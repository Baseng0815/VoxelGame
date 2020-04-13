#pragma once

#include "System.h"

class Camera;

// handles player movement input
// also updates the camera
class InputSystem : public System {
    private:
        void handleKeyPressEvent(Event* e);
        void handleMouseMoveEvent(Event* e);
        void handleScrollEvent(Event* e);

    public:
        InputSystem(SystemManager* systemManager, SharedContext* context);

        void init() override;
        void update(int dt) override;

        ~InputSystem() = default;

};
