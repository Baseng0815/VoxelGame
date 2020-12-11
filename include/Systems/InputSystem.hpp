#pragma once

#include <glm/glm.hpp>

#include "System.hpp"
#include "../Events/CallbackHandle.hpp"

struct CameraComponent;
struct TransformationComponent;
struct VelocityComponent;

struct KeyEvent;
struct MouseButtonEvent;
struct CursorEvent;
struct ScrollEvent;
struct FramebufferSizeEvent;

class InventoryLayout;
class HotbarLayout;

// handles player movement input
// also updates the camera
// TODO maybe put camera stuff in extra system?
class InputSystem : public System {
    private:
        struct LookData { glm::vec3 block; glm::vec3 face; bool valid; };
        entt::entity m_player;

        int m_lastKey;
        int m_lastKeyTime = 0;

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

        void updateVectors(CameraComponent &camera);
        void updateViewMatrix(CameraComponent &camera);
        void updateProjectionMatrix(CameraComponent &camera);

        void _update(int dt) override;

        LookData selectedBlock;
        InventoryLayout &m_inventoryLayout;
        HotbarLayout    &m_hotbarLayout;

        byte playerInputState = 0;
    public:
        InputSystem(Registry_T &registry, InventoryLayout &inventoryLayout, HotbarLayout &m_hotbarLayout);
};
