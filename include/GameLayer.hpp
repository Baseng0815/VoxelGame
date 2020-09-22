#pragma once

#include "Gui/GUI.hpp"
#include "Systems/System.hpp"

class Application;

class GameLayer {
    protected:
        Application* m_application;

    public:
        GameLayer(Application* application);

        virtual void update(int dt) = 0;
};
