#include "GameLayer.h"

#include <vector>
#include <atomic>
#include <entt/entt.hpp>

#include "Gui/GUI.h"

class IngameLayer : public GameLayer {
    private:
        std::vector<System*> m_systems;
        entt::registry m_registry;

        GUI m_gui;

    public:
        IngameLayer(Application* application);
        ~IngameLayer();

        void update(int dt) override;

        void setDebugInfo(int fps, int rendertime, int chunkCount);
};
