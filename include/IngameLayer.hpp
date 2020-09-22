#pragma once

#include "GameLayer.hpp"

#include <vector>
#include <atomic>
#include <entt/entt.hpp>

#include "Gui/IngameLayerGUI.hpp"

#include "Events/CallbackHandle.hpp"

class IngameLayer : public GameLayer {
    private:
        std::vector<std::unique_ptr<System>> m_systems;
        Registry_T m_registry;

        CallbackHandle<const KeyEvent&> m_keyEventHandle;
        void handleKeys(const KeyEvent&);

        int m_time = 0;
        int m_frameCounter = 0;

        IngameLayerGUI m_gui;

    public:
        IngameLayer(Application *application);
        ~IngameLayer();

        void update(int dt) override;
};
