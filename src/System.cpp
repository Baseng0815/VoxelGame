#include "../include/System.h"
#include "../include/SystemManager.h"
#include "../include/SharedContext.h"

System::System(SystemManager* systemManager, SharedContext* context)
    : m_systemManager(systemManager), m_registry(&systemManager->getRegistry()), m_context(context) {}
