#pragma once

#include "System.h"

class Camera;
class CameraComponent;
class TransformationComponent;
class VelocityComponent;

// handles player movement input
// also updates the camera
class InputSystem : public System {
    private:
        void handleKeyPressEvent(Event* e);
        void handleMouseMoveEvent(Event* e);
        void handleScrollEvent(Event* e);
        void handleFramebufferSizeEvent(Event* e);
        
        void updateVectors(CameraComponent& camera);        

        void _update(int dt) override;

    public:
        InputSystem(SystemManager* systemManager);
};
