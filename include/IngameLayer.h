#include "GameLayer.h"

#include <vector>
#include <atomic>
#include <entt/entt.hpp>

class IngameLayer : public GameLayer {
    private:
        std::vector<System*> m_systems;

        entt::registry m_registry;

    public:
        IngameLayer();
        ~IngameLayer();

        void update(int dt) override;
};
