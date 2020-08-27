#pragma once

#include <glm/glm.hpp>

#include "System.h"

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

        CallbackHandle<const KeyEvent&> m_keyPressHandle;
        void handleKeyPressEvent(const KeyEvent&);
        CallbackHandle<const MouseButtonEvent&> m_mouseButtonHandle;
        void handleMouseButtonEvent(const MouseButtonEvent&);
        CallbackHandle<const CursorEvent&> m_cursorHandle;
        void handleMouseMoveEvent(const CursorEvent&);
        CallbackHandle<const ScrollEvent&> m_scrollHandle;
        void handleScrollEvent(const ScrollEvent&);
        CallbackHandle<const FramebufferSizeEvent&> m_framebufferHandle;
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
