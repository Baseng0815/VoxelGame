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

void InputSystem::handleFramebufferSizeEvent(Event* e) {
    FramebufferSizeEvent sizeEvent = *e->get<FramebufferSizeEvent>();

    m_context->camera.handleFramebufferSize(sizeEvent.width, sizeEvent.height);
}

InputSystem::InputSystem(SystemManager* systemManager, SharedContext* sharedContext)
    : System(systemManager, sharedContext) {
    ADD_EVENT(handleKeyPressEvent, KEY_EVENT);
    ADD_EVENT(handleMouseMoveEvent, CURSOR_EVENT);
    ADD_EVENT(handleScrollEvent, SCROLL_EVENT);
    ADD_EVENT(handleFramebufferSizeEvent, FRAMEBUFFER_SIZE_EVENT);
}

void InputSystem::update(int dt) {
    m_context->camera.update(dt);
}
