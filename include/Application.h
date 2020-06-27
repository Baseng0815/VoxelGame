#pragma once

#include "Rendering/Window.h"

struct KeyEvent;
class GameLayer;

class Application {
private:
    Window m_window;
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

    void setNewLayer(GameLayer* newLayer);

    // enter game loop
    void run();

    // set isRunning to false
    void stop();
};
