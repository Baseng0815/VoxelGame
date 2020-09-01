#include "../../include/Systems/System.h"

#include "../../include/Events/EventDispatcher.h"

System::System(Registry_T &registry, int updateDelay)
    : m_registry {registry}, m_updateDelay {updateDelay} {}

void System::update(int dt) {
    m_currentTime += dt;
    if (m_currentTime >= m_updateDelay) {
        m_currentTime = 0;
        _update(dt);
    }
}
