#include "../include/MainMenuLayer.h"

#include "../include/Application.h"

MainMenuLayer::MainMenuLayer(Application* application)
    : GameLayer(application) {
    application->getWindow().enableCursor();
}

MainMenuLayer::~MainMenuLayer() {

}

void MainMenuLayer::update(int dt) {
    m_mainMenu.update();
    m_mainMenu.draw();
}
