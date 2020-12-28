#pragma once

#include <glm/glm.hpp>

#include "System.hpp"

struct CameraComponent;
struct ChunkComponent;
struct TransformationComponent;
struct VelocityComponent;

struct CursorEvent;
struct FramebufferSizeEvent;
struct KeyEvent;
struct MouseButtonEvent;
struct ScrollEvent;

class InventoryLayout;
class HotbarLayout;

// updates the camera
class CameraSystem : public System {
    private:
        entt::entity m_player;

        CallbackHandle<const CursorEvent&> m_cursorHandle;
        void handleMouseMoveEvent(const CursorEvent&);
        CallbackHandle<const FramebufferSizeEvent&> m_framebufferHandle;
        void handleFramebufferSizeEvent(const FramebufferSizeEvent&);

        void updateVectors(CameraComponent &camera);
        void updateViewMatrix(CameraComponent &camera);
        void updateProjectionMatrix(CameraComponent &camera);

        bool isVisible(const glm::vec3 &camPos, float fov_x, int x, int z);

        void _update(int dt) override;
    public:
        CameraSystem(Registry_T &registry);
};
