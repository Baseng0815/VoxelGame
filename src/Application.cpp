#include "../include/Application.hpp"

//#include "../include/Configuration.hpp"
//#include "../include/Events/Event.hpp"
#include "../include/Events/EventDispatcher.hpp"

#include "../include/Resources/ResourceManager.hpp"

// TODO put layers into own folder
#include "../include/IngameLayer.hpp"
#include "../include/MainMenuLayer.hpp"

#include "../include/GameData/GameData.hpp"

#include "../include/Components/CameraComponent.hpp"

#include <chrono>
//#include <iostream>

void Application::handleKeys(const KeyEvent& e) {
    switch (e.key) {
        case GLFW_KEY_ESCAPE:
            if (e.action == GLFW_PRESS)
                m_isRunning = false;
            break;

        case GLFW_KEY_F:
            if (e.action == GLFW_PRESS)
                m_window.toggleFullscreen();
            break;

        default:
            break;
    }
}

Application::Application()
    : m_window(this, 800, 600)
{
    srand(time(NULL));

    ResourceManager::loadResources();
    GameData::loadGameData();

    EventDispatcher::attachToWindow(m_window);

    m_keyEventHandle = EventDispatcher::onKeyPress.subscribe([&](const KeyEvent& e) {
        handleKeys(e);
    });

    m_currentLayer = new IngameLayer(this);
    //m_currentLayer = new MainMenuLayer(this);
}

Window& Application::getWindow()
{ return m_window; }

Application::~Application()
{
    m_window.close();
    glfwTerminate();
}

void Application::setNewLayer(GameLayer* newLayer)
{ m_currentLayer = newLayer; }

void Application::run()
{
    while (m_isRunning) {
        // event polling
        glfwPollEvents();

        // fps and render time
        if (m_time > 1000) {
            std::cout << m_frameCounter / (float)m_time * 1000 << "fps, "
                << m_time / (float)m_frameCounter << "ms avg. frame render time" << std::endl;

            m_frameTime = 0;
            m_frameCounter = 0;
            m_time = 0;
        }

        // time
        int currentTime = glfwGetTime() * 1000.f;
        m_deltaTime = std::min(currentTime - m_prevTime, 200);
        m_prevTime = currentTime;
        m_time += m_deltaTime;

        auto startTime = std::chrono::high_resolution_clock::now();

        m_window.clear();
        m_currentLayer->update(m_deltaTime);
        m_window.display();

        auto endTime = std::chrono::high_resolution_clock::now();
        m_frameTime += std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        m_frameCounter++;
    }
}

void Application::stop()
{ m_isRunning = false; }
