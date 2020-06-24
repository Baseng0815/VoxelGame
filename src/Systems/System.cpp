#include "../../include/Systems/System.h"

#include "../../include/Events/EventDispatcher.h"

System::System(entt::registry* systemManager, int updateDelay)
    : m_registry(systemManager), m_updateDelay(updateDelay) {}

void System::update(int dt) {
    m_currentTime += dt;
    if (m_currentTime > m_updateDelay) {
        m_currentTime = 0;
        _update(dt);
    }
}
