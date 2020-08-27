#include "GameLayer.h"

#include <vector>
#include <atomic>
#include <entt/entt.hpp>

#include "Gui/GUI.h"

class IngameLayer : public GameLayer {
    private:
        std::vector<std::unique_ptr<System>> m_systems;
        entt::registry m_registry;

        int m_time = 0;
        int m_frameCounter = 0;

        GUI m_gui;

    public:
        IngameLayer(Application* application);
        ~IngameLayer();

        void update(int dt) override;
};
