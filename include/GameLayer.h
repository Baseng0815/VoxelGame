#pragma once

#include "Gui/GUI.h"
#include "Systems/System.h"

class Application;

class GameLayer {
    protected:
        Application* m_application;

    public:
        GameLayer(Application* application);

        virtual void update(int dt) = 0;
};
