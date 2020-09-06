#include "../../include/Systems/System.h"

#include "../../include/Events/EventDispatcher.h"

System::System(Registry_T &registry, int updateDelay)
    : m_registry {registry}, m_updateDelay {updateDelay} {}

void System::update(int dt) {
    m_timeSinceLastUpdate += dt;
    if (m_timeSinceLastUpdate >= m_updateDelay) {
        _update(m_timeSinceLastUpdate);
        m_timeSinceLastUpdate = 0;
    }
}
