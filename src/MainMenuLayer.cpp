#include "../include/MainMenuLayer.h"

#include "../include/IngameLayer.h"
#include "../include/Application.h"

MainMenuLayer::MainMenuLayer(Application* application)
    : GameLayer(application) {
    application->getWindow().enableCursor();

    m_mainMenu.getWidget("button_start").onPress += [&](int x, int y) {
        m_application->setNewLayer(new IngameLayer(m_application));
    };
    m_mainMenu.getWidget("button_quit").onPress += [&](int x, int y) {
        m_application->stop();
    };
}

void MainMenuLayer::update(int dt) {
    m_mainMenu.update();
    m_mainMenu.draw();
}
