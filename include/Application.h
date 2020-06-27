#pragma once

#include "Gui/GUI.h"
#include "Rendering/Window.h"

#include "MainMenuLayer.h"

struct KeyEvent;

class Application {
private:
    Window m_window;
    GUI m_gui;

    GameLayer* m_currentLayer;

    // delta time
    int m_prevTime = 0, m_deltaTime = 0;

    // fps
    int m_time = 0, m_frameCounter = 0;
    bool m_isRunning = true;

    // frametime, in microseconds, only related to the rendering process
    int m_frameTime = 0;

    void handleKeys(const KeyEvent&);

public:
    Application();
    ~Application();

    Window& getWindow();

    // enter game loop
    void run();

    // set isRunning to false
    void stop();
};
