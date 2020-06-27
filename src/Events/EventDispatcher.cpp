#include "../../include/Events/EventDispatcher.h"

#include "../../include/Application.h"

bool EventDispatcher::m_firstMouse = true;
double EventDispatcher::m_prevX = 0;
double EventDispatcher::m_prevY = 0;

glm::vec2 EventDispatcher::m_framebufferSize = {0, 0};

CallbackList<KeyEvent> EventDispatcher::onKeyPress;
CallbackList<MouseButtonEvent> EventDispatcher::onMouseButtonPress;
CallbackList<CursorEvent> EventDispatcher::onCursorMove;
CallbackList<ScrollEvent> EventDispatcher::onMouseScroll;
CallbackList<FramebufferSizeEvent> EventDispatcher::onFramebufferSize;

CallbackList<EnterChunkEvent> EventDispatcher::onEnterChunk;
CallbackList<BlockChangedEvent> EventDispatcher::onBlockChange;

glm::vec2 EventDispatcher::getFramebufferSize() {
    return m_framebufferSize;
}

void EventDispatcher::attachToWindow(const Window& window) {
    GLFWwindow* hwnd = window.getHandle();

    glfwSetKeyCallback(hwnd, EventDispatcher::keyCallback);
    glfwSetCursorPosCallback(hwnd, EventDispatcher::cursorPosCallback);
    glfwSetScrollCallback(hwnd, EventDispatcher::scrollCallback);
    glfwSetFramebufferSizeCallback(hwnd, EventDispatcher::framebufferSizeCallback);
    glfwSetMouseButtonCallback(hwnd, EventDispatcher::mouseButtonCallback);
}

void EventDispatcher::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* application = (Application*)glfwGetWindowUserPointer(window);

    KeyEvent e(application, key, scancode, action, mods);
    raiseEvent(e);
}

void EventDispatcher::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Application* application = (Application*)glfwGetWindowUserPointer(window);

    MouseButtonEvent e(application, button, action, mods);
    raiseEvent(e);
}

void EventDispatcher::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (m_firstMouse) {
        m_prevX = xpos;
        m_prevY = ypos;
        m_firstMouse = false;
        return;
    }

    double dx = xpos - m_prevX;
    double dy = ypos - m_prevY;

    Application* application = (Application*)glfwGetWindowUserPointer(window);

    CursorEvent e(application, xpos, ypos, dx, dy);
    raiseEvent(e);

    m_prevX = xpos;
    m_prevY = ypos;
}

void EventDispatcher::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Application* application = (Application*)glfwGetWindowUserPointer(window);

    ScrollEvent e(application, xoffset, yoffset);
    raiseEvent(e);
}

void EventDispatcher::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Application* application = (Application*)glfwGetWindowUserPointer(window);

    FramebufferSizeEvent e(application, width, height);
    m_framebufferSize = {width, height};
    raiseEvent(e);
}

void EventDispatcher::resetMouseState() {
    m_firstMouse = true;
}

void EventDispatcher::raiseEvent(const KeyEvent& e) {
    onKeyPress.invoke(e);
}

void EventDispatcher::raiseEvent(const MouseButtonEvent& e) {
    onMouseButtonPress.invoke(e);
}

void EventDispatcher::raiseEvent(const CursorEvent& e) {
    onCursorMove.invoke(e);
}

void EventDispatcher::raiseEvent(const ScrollEvent& e) {
    onMouseScroll.invoke(e);
}

void EventDispatcher::raiseEvent(const FramebufferSizeEvent& e) {
    onFramebufferSize.invoke(e);
}

void EventDispatcher::raiseEvent(const EnterChunkEvent& e) {
    onEnterChunk.invoke(e);
}

void EventDispatcher::raiseEvent(const BlockChangedEvent& e) {
    onBlockChange.invoke(e);
}