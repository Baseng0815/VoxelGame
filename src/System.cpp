#include "../include/System.h"
#include "../include/SystemManager.h"

System::System(SystemManager* systemManager, int updateDelay)
    : m_systemManager(systemManager), m_updateDelay(updateDelay) {}

void System::update(int dt) {
    m_currentTime += dt;
    if (m_currentTime > m_updateDelay) {
        m_currentTime = 0;
        _update(dt);
    }
}
