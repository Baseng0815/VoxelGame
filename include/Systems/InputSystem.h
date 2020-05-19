#pragma once

#include "System.h"
#include <glm/glm.hpp>

class Camera;
class CameraComponent;
class TransformationComponent;
class VelocityComponent;


// handles player movement input
// also updates the camera
class InputSystem : public System {
    private:
        struct LookData { glm::ivec3 block; glm::ivec3 face; bool valid; };

        void handleKeyPressEvent(Event* e);
        void handleMouseButtonEvent(Event* e);
        void handleMouseMoveEvent(Event* e);
        void handleScrollEvent(Event* e);
        void handleFramebufferSizeEvent(Event* e);               

        void updateVectors(CameraComponent& camera);   
        void updateSelectedBlock(CameraComponent& camera, TransformationComponent& transformation);

        void _update(int dt) override;

        int lastSpacePress = INT_MAX;
        LookData selectedBlock;

    public:
        InputSystem(SystemManager* systemManager);
};
