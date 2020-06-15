#include "../include/MainMenuLayer.h"

MainMenuLayer::MainMenuLayer() {

}

MainMenuLayer::~MainMenuLayer() {

}

void MainMenuLayer::update(int dt) {
    m_mainMenu.update();
    m_mainMenu.draw();
}
