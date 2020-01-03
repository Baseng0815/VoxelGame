#include "../include/Window.h"
#include "../include/Definitions.h"
#include "../include/EventDispatcher.h"

void Window::init(Application* app) {
    glfwInit();

    m_window = glfwCreateWindow(Definitions::WINDOW_WIDTH, Definitions::WINDOW_HEIGHT, "GLFW window", nullptr, nullptr);
    if (!m_window)
        throw std::runtime_error("Failed to create GLFW window.");

    m_primaryMonitor = glfwGetPrimaryMonitor();

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(m_window, app);

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
}

void Window::clear(Color clearColor) {
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

void Window::close() {
    glfwDestroyWindow(m_window);
}
