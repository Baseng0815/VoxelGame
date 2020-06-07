#include "../include/EventDispatcher.h"
#include "../include/Application.h"

bool EventDispatcher::m_firstMouse = true;
double EventDispatcher::m_prevX = 0;
double EventDispatcher::m_prevY = 0;
CallbackId EventDispatcher::m_cbCounter = 0;
std::map<EventType, std::vector<Callback>> EventDispatcher::m_callbacks;

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
    dispatch(&e);
}

void EventDispatcher::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Application* application = (Application*)glfwGetWindowUserPointer(window);

    MouseButtonEvent e(application, button, action, mods);
    dispatch(&e);
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
    dispatch(&e);

    m_prevX = xpos;
    m_prevY = ypos;
}

void EventDispatcher::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Application* application = (Application*)glfwGetWindowUserPointer(window);

    ScrollEvent e(application, xoffset, yoffset);
    dispatch(&e);
}

void EventDispatcher::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Application* application = (Application*)glfwGetWindowUserPointer(window);

    FramebufferSizeEvent e(application, width, height);
    dispatch(&e);
}

void EventDispatcher::dispatch(Event *e) {
    auto cbIt = m_callbacks.find(e->type());
    if (cbIt == m_callbacks.end()) return;

    for (auto& callback : (*cbIt).second)
        if (callback.isActive)
            callback.fun(e);
}

CallbackId EventDispatcher::addCallback(CallbackFun fun, EventType type) {
    m_cbCounter++;

    Callback callback(fun, type);
    callback.id = m_cbCounter;

    auto listPair = m_callbacks.insert(std::make_pair(callback.type, std::vector<Callback>()));
    listPair.first->second.push_back(callback);

    return callback.id;
}

void EventDispatcher::removeCallback(CallbackId callbackId) {
    for (auto& [key, val] : m_callbacks) {
        auto it = std::find(val.begin(), val.end(), callbackId);
        if (it != val.end()) {
            val.erase(it);
            return;
        }
    }
}

void EventDispatcher::setCallbackActive(CallbackId callbackId, bool active) {
    for (auto& [key, val] : m_callbacks) {
        auto it = std::find(val.begin(), val.end(), callbackId);
        if (it != val.end()) {
            it->isActive = active;
            return;
        }
    }
}

void EventDispatcher::resetMouseState() {
    m_firstMouse = true;
}

void EventDispatcher::raiseEvent(Event* e) {
    dispatch(e);
}
