#include "../../include/Rendering/Window.h"
#include "../../include/Configuration.h"
#include "../../include/EventDispatcher.h"

#ifdef WIN32
#include <stdexcept>
#endif

Window::Window(Application* app) {
    Configuration::loadConfiguration("Resources/");
    glfwInit();

    m_window = glfwCreateWindow(Configuration::getFloatValue("WINDOW_WIDTH"), Configuration::getFloatValue("WINDOW_HEIGHT"), "GLFW window", nullptr, nullptr);
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

bool Window::getKey(int key) const {
    return glfwGetKey(m_window, key);
}

GLFWwindow* Window::getHandle() const {
    return m_window;
}

Event* Window::nextEvent() {
    Event* e = m_events.front();
    m_events.pop();
    return e;
}

void Window::close() {
    glfwDestroyWindow(m_window);
}