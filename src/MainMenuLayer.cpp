#include "../include/MainMenuLayer.hpp"

#include "../include/IngameLayer.hpp"
#include "../include/Application.hpp"
#include "../include/Gui/MainMenuLayout.hpp"

MainMenuLayer::MainMenuLayer(Application* application)
    : GameLayer(application) {
    application->getWindow().enableCursor();

    m_mainMenu.addPanel(new MainMenuLayout(m_mainMenu));

    m_btnStartHandle = m_mainMenu.getWidget("button_start").onPress.subscribe([&](const MouseButtonEvent&) {
        m_application->setNewLayer(new IngameLayer(m_application));
    });

    m_btnQuitHandle = m_mainMenu.getWidget("button_quit").onPress.subscribe([&](const MouseButtonEvent&) {
        m_application->stop();
    });
}

void MainMenuLayer::update(int dt) {
    m_mainMenu.update();
    m_mainMenu.draw();
}
