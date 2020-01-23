#include "../include/System.h"

System::System(SystemManager* systemManager)
    : m_systemManager(systemManager) {}

void System::setRegistry(entt::registry* r) {
    m_registry = r;
}
