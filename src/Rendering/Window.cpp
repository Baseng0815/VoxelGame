#include "../../include/Rendering/Window.h"
#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"

#include <glm/gtc/matrix_transform.hpp>

#ifdef WIN32
#include <stdexcept>
#endif

// TODO prevent camera from receiving events when ALT is pressed
void Window::handleKeyPress(Event* e) {
    KeyEvent keyEvent = *e->get<KeyEvent>();
    if (keyEvent.key == GLFW_KEY_LEFT_ALT) {
        if (keyEvent.action == GLFW_PRESS) {
            enableCursor();
        } else if (keyEvent.action == GLFW_RELEASE) {
            disableCursor();
        }
    }
}

void Window::handleFramebufferSize(Event* e) {
    FramebufferSizeEvent event = *e->get<FramebufferSizeEvent>();
    glViewport(0, 0, event.width, event.height);
}

Window::Window(Application* app, int width, int height) {
    Configuration::loadConfiguration("Resources/");
    glfwInit();

    m_window = glfwCreateWindow(width, height, "GLFW window", nullptr, nullptr);
    if (!m_window)
        throw std::runtime_error("Failed to create GLFW window.");

    m_primaryMonitor = glfwGetPrimaryMonitor();

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(m_window, app);

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0);

    // init glew and load function pointers
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    EventDispatcher::addCallback(CB_FUN(handleKeyPress), KEY_EVENT);
    EventDispatcher::addCallback(CB_FUN(handleFramebufferSize), FRAMEBUFFER_SIZE_EVENT);
}

void Window::clear(glm::vec3 clearColor) {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Window::display() {
    glfwSwapBuffers(m_window);
}

void Window::toggleFullscreen() {
    if (m_isFullscreen) {
        glfwSetWindowPos(m_window, m_prevX, m_prevY);
        glfwSetWindowSize(m_window, m_prevWidth, m_prevHeight);

        glViewport(0, 0, m_prevWidth, m_prevHeight);

        m_isFullscreen = false;
    }
    else {
        glfwGetWindowSize(m_window, &m_prevWidth, &m_prevHeight);
        glfwGetWindowPos(m_window, &m_prevX, &m_prevY);

        const GLFWvidmode* vidMode = glfwGetVideoMode(m_primaryMonitor);
        glfwSetWindowPos(m_window, 0, 0);
        glfwSetWindowSize(m_window, vidMode->width, vidMode->height);

        glViewport(0, 0, vidMode->width, vidMode->height);

        m_isFullscreen = true;
    }
}

void Window::enableCursor() {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::disableCursor() {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

bool Window::getKey(int key) const {
    return glfwGetKey(m_window, key);
}

GLFWwindow* Window::getHandle() const {
    return m_window;
}

void Window::close() {
    glfwDestroyWindow(m_window);
}
