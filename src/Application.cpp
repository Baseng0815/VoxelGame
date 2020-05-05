#include "../include/Application.h"
#include "../include/EventDispatcher.h"

#include <iostream>

void Application::handleKeys(Event* event) {
    KeyEvent* kE = event->get<KeyEvent>();

    switch (kE->key) {
        case GLFW_KEY_ESCAPE:
            if (kE->action == GLFW_PRESS)
                m_isRunning = false;
            break;

        case GLFW_KEY_F:
            if (kE->action == GLFW_PRESS)
                m_window.toggleFullscreen();
            break;

        default:
            break;
    }
}

Application::Application()
    : m_window(this), m_systemManager(&m_context) {
    srand(time(NULL));

    EventDispatcher::attachToWindow(m_window);
    EventDispatcher::addCallback(std::bind(&Application::handleKeys, this, std::placeholders::_1), EventType::KEY_EVENT);
}

Application::~Application() {
    m_window.close();
    glfwTerminate();
}

void Application::run() {
    while (m_isRunning) {
        // event polling
        glfwPollEvents();

        // fps and render time
        if (m_time > 1000) {
            std::cout << m_frameCounter / (float)m_time * 1000 << " fps" << std::endl;
            m_frameTime = 0;
            m_frameCounter = 0;
            m_time = 0;
        }

        // time
        int currentTime = glfwGetTime() * 1000.f;
        m_deltaTime = currentTime - m_prevTime;
        m_prevTime = currentTime;
        m_time += m_deltaTime;

        // updating and drawing
        auto startTime = std::chrono::high_resolution_clock::now();

        m_window.clear();
        m_systemManager.update(m_deltaTime);
        m_window.display();

        auto endTime = std::chrono::high_resolution_clock::now();
        m_frameTime += std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        m_frameCounter++;
    }
}

void Application::stop() {
	m_isRunning = false;
}
