#include "../include/InputSystem.h"

#include "../include/Camera.h"
#include "../include/SharedContext.h"
#include "../include/EventDispatcher.h"

void InputSystem::handleKeyPressEvent(Event* e) {
    KeyEvent keyEvent = *e->get<KeyEvent>();

    // TODO add player
    m_context->camera.handle

}

void InputSystem::handleMouseMoveEvent(Event* e) {

}

InputSystem::InputSystem(SystemManager* systemManager, SharedContext* context)
    : System(systemManager, context) {}

void InputSystem::init() {
    ADD_EVENT(InputSystem::handleKeyPressEvent, KEY_EVENT);
    ADD_EVENT(InputSystem::handleMouseMoveEvent, CURSOR_EVENT);
}
