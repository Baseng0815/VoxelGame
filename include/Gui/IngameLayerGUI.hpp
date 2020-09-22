#pragma once

#include "GUI.hpp"

class DebugLayout;
class DebugLayoutInfo;

class IngameLayerGUI : public GUI {
    private:
        DebugLayout *m_debugLayout;

    public:
        IngameLayerGUI();

        void setDebugInfo(const DebugLayoutInfo &info);
};
