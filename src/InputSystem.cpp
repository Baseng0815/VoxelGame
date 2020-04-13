#include "../include/InputSystem.h"

#include "../include/Camera.h"
#include "../include/SharedContext.h"
#include "../include/EventDispatcher.h"

void InputSystem::handleKeyPressEvent(Event* e) {
    KeyEvent keyEvent = *e->get<KeyEvent>();

    m_context->camera.handleKey(keyEvent.key, keyEvent.action);
}

void InputSystem::handleMouseMoveEvent(Event* e) {
    CursorEvent cursorEvent = *e->get<CursorEvent>();

    m_context->camera.handleCursorPos(cursorEvent.dx, cursorEvent.dy);
}

void InputSystem::handleScrollEvent(Event* e) {
    ScrollEvent scrollEvent = *e->get<ScrollEvent>();

    m_context->camera.handleScroll(scrollEvent.dy);
}

InputSystem::InputSystem(SystemManager* systemManager, SharedContext* context)
    : System(systemManager, context) {}

void InputSystem::init() {
    ADD_EVENT(InputSystem::handleKeyPressEvent, KEY_EVENT);
    ADD_EVENT(InputSystem::handleMouseMoveEvent, CURSOR_EVENT);
    ADD_EVENT(InputSystem::handleScrollEvent, SCROLL_EVENT);
}

void InputSystem::update(int dt) {
    m_context->camera.update(dt);
}
