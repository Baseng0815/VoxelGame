#pragma once

#include "System.hpp"
#include "../Configuration.hpp"

class Shader;

// handles day/night cycle
// each cycle is divided into 24000 units
class DayNightSystem : public System {
    private:
        entt::entity m_player;
        entt::entity m_skybox;

        static constexpr float m_ingameTimePerRealMillis = 24000 / (float)Configuration::DAYNIGHT_DURATION / 1000.f;
        float m_ingameTime = 8000;

        void _update(int dt) override;

    public:
        DayNightSystem(Registry_T &registry);
};
