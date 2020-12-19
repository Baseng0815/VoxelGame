#include "../../include/Systems/DayNightSystem.hpp"

#include "../../include/Components/SkyboxComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"
#include "../../include/Components/LightComponent.hpp"

#include "../../include/Utility.hpp"

#include <iostream>
void DayNightSystem::_update(int dt)
{
    // m_ingameTime += dt * m_ingameTimePerRealMillis;
    // m_ingameTime = std::fmod(m_ingameTime, 24000.f);

    // float angle = 2 * Utility::PI * m_ingameTime / 24000.f;
    float angle = 0.25 * Utility::PI;
    LightingComponent& lighting = m_registry.raw<LightingComponent>()[0];
    lighting.dirLights[0].direction.x = -std::cos(angle);
    lighting.dirLights[0].direction.y = -std::sin(angle);

    TransformationComponent &transform = m_registry.get<TransformationComponent>(m_skybox);
    transform.setRotation(glm::angleAxis(angle, glm::vec3 {0.f, 0.f, 1.f}));
}

DayNightSystem::DayNightSystem(Registry_T &registry)
    : System {registry}, m_player {m_registry.view<PlayerComponent>().front()},
    m_skybox {m_registry.view<SkyboxComponent>().front()}
{ }
