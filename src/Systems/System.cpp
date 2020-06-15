#include "../../include/Systems/System.h"

#include "../../include/EventDispatcher.h"

System::System(entt::registry* systemManager, int updateDelay)
    : m_registry(systemManager), m_updateDelay(updateDelay) {}

System::~System() {
    for (auto id : m_callbacks)
        EventDispatcher::removeCallback(id);
}

void System::update(int dt) {
    m_currentTime += dt;
    if (m_currentTime > m_updateDelay) {
        m_currentTime = 0;
        _update(dt);
    }
}
