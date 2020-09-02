#pragma once

#include "GameLayer.h"
#include "Gui/GUI.h"

class MainMenuLayer : public GameLayer {
    private:
        GUI m_mainMenu;
        CallbackHandle<const MouseButtonEvent&> m_btnStartHandle;
        CallbackHandle<const MouseButtonEvent&> m_btnQuitHandle;

    public:
        MainMenuLayer(Application* application);
        ~MainMenuLayer() = default;

        void update(int dt) override;
};
