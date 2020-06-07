#include "../../include/Systems/System.h"
#include "../../include/Systems/SystemManager.h"

#include "../../include/EventDispatcher.h"

System::System(SystemManager* systemManager, int updateDelay)
    : m_systemManager(systemManager), m_updateDelay(updateDelay) {}

void System::update(int dt) {
    m_currentTime += dt;
    if (m_currentTime > m_updateDelay) {
        m_currentTime = 0;
        _update(dt);
    }
}

void System::detachEvents() const {
    for (auto id : m_callbacks)
        EventDispatcher::setCallbackActive(id, false);
}

void System::attachEvents() const {
    for (auto id : m_callbacks)
        EventDispatcher::setCallbackActive(id, true);
}
