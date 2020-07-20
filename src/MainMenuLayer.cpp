#include "../include/MainMenuLayer.h"

#include "../include/IngameLayer.h"
#include "../include/Application.h"
#include "../include/Gui/MainMenuLayout.h"

MainMenuLayer::MainMenuLayer(Application* application)
    : GameLayer(application) {
    application->getWindow().enableCursor();

    m_mainMenu.addPanel(new MainMenuLayout(&m_mainMenu));

    m_mainMenu.getWidget("button_start").onPress += [&](const MouseButtonEvent&) {
        m_application->setNewLayer(new IngameLayer(m_application));
    };
    m_mainMenu.getWidget("button_quit").onPress += [&](const MouseButtonEvent&) {
        m_application->stop();
    };
}

void MainMenuLayer::update(int dt) {
    m_mainMenu.update();
    m_mainMenu.draw();
}
