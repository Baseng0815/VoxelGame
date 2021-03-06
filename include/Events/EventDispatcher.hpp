#pragma once

#include "CallbackList.hpp"
#include "Event.hpp"

#include "../Rendering/Window.hpp"

#include <vector>

class EventDispatcher {
    private:
        static bool m_firstMouse;
        static double m_prevX, m_prevY;

        static glm::vec2 m_framebufferSize;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    public:
        static glm::vec2 getFramebufferSize();

        static void attachToWindow(const Window& window);

        // TODO somehow do this using templates maybe?
        static CallbackList<const KeyEvent&> onKeyPress;
        static CallbackList<const MouseButtonEvent&> onMouseButtonPress;
        static CallbackList<const CursorEvent&> onCursorMove;
        static CallbackList<const ScrollEvent&> onMouseScroll;
        static CallbackList<const FramebufferSizeEvent&> onFramebufferSize;

        static CallbackList<const EnterChunkEvent&> onEnterChunk;
        static CallbackList<const EntityMovedEvent&> onEntityMoved;
        static CallbackList<const BlockChangedEvent&> onBlockChange;
        static CallbackList<const BlockCollisionEvent&> onBlockCollision;

        static CallbackList<DragEvent<std::tuple<BlockId, int>>&> onItemDragStart;
        static CallbackList<const DragEvent<std::tuple<BlockId, int>>&> onItemDrop;

        static void
            raiseEvent(const KeyEvent&);
        static void raiseEvent(const MouseButtonEvent&);
        static void raiseEvent(const CursorEvent&);
        static void raiseEvent(const ScrollEvent&);
        static void raiseEvent(const FramebufferSizeEvent&);
        static void raiseEvent(const EnterChunkEvent&);
        static void raiseEvent(const EntityMovedEvent&);
        static void raiseEvent(const BlockChangedEvent&);
        static void raiseEvent(const BlockCollisionEvent&);
        static void raiseItemDragStartEvent(DragEvent<std::tuple<BlockId, int>>&);
        static void raiseItemDropEvent(const DragEvent<std::tuple<BlockId, int>>&);

        // sets mouse state to firstMouse so the first input event is ignored
        static void resetMouseState();
};
