#include "../../include/Systems/System.hpp"

#include "../../include/Events/EventDispatcher.hpp"

System::System(Registry_T &registry)
    : m_registry {registry}
{  }

void System::update(int dt) {
    _update(dt);
}
