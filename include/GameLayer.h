#pragma once

#include "Gui/GUI.h"
#include "Systems/System.h"

class GameLayer {
    public:
        virtual void update(int dt) = 0;
};
