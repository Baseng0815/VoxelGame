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
        void handleFramebufferSizeEvent(Event* e);

    public:
        InputSystem(SystemManager* systemManager, SharedContext* context);

        void update(int dt) override;

};
