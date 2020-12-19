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
class InputSystem : public System {
    private:
        entt::entity m_player;

        int m_lastKey;
        int m_lastKeyTime = 0;

        CallbackHandle<const KeyEvent&> m_keyPressHandle;
        void handleKeyPressEvent(const KeyEvent&);
        CallbackHandle<const MouseButtonEvent&> m_mouseButtonHandle;
        void handleMouseButtonEvent(const MouseButtonEvent&);
        CallbackHandle<const ScrollEvent&> m_scrollHandle;
        void handleScrollEvent(const ScrollEvent&);

        void _update(int dt) override;

        // TODO maybe put this somewhere else
        InventoryLayout &m_inventoryLayout;
        HotbarLayout    &m_hotbarLayout;

        byte playerInputState = 0;
    public:
        InputSystem(Registry_T &registry, InventoryLayout &inventoryLayout, HotbarLayout &m_hotbarLayout);
};
