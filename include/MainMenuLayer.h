#pragma once

#include "GameLayer.h"
#include "Gui/MainMenu.h"

class MainMenuLayer : public GameLayer {
    private:
        MainMenu m_mainMenu;

    public:
        MainMenuLayer(Application* application);
        ~MainMenuLayer() = default;

        void update(int dt) override;
};
