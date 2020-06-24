#pragma once

#include "System.h"
#include <glm/glm.hpp>

class Camera;
class CameraComponent;
class TransformationComponent;
class VelocityComponent;

struct KeyEvent;
struct MouseButtonEvent;
struct CursorEvent;
struct ScrollEvent;
struct FramebufferSizeEvent;

// handles player movement input
// also updates the camera
class InputSystem : public System {
    private:
        struct LookData { glm::vec3 block; glm::vec3 face; bool valid; };

        void handleKeyPressEvent(const KeyEvent&);
        void handleMouseButtonEvent(const MouseButtonEvent&);
        void handleMouseMoveEvent(const CursorEvent&);
        void handleScrollEvent(const ScrollEvent&);
        void handleFramebufferSizeEvent(const FramebufferSizeEvent&);

        void updateSelectedBlock(CameraComponent& camera, TransformationComponent& transformation);

        void updateVectors(CameraComponent& camera);
        void updateViewMatrix(CameraComponent& camera, TransformationComponent& transform);
        void updateProjectionMatrix(CameraComponent& camera);

        void _update(int dt) override;

        int lastSpacePress = INT_MAX;
        LookData selectedBlock;

    public:
        InputSystem(entt::registry* registry);
};
