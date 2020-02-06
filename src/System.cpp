#include "../include/System.h"
#include "../include/SystemManager.h"

System::System(SystemManager* systemManager)
    : m_systemManager(systemManager), m_registry(&systemManager->getRegistry()) {}
