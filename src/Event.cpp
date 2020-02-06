#include "../include/Event.h"

template<class T>
T* Event::get() {
    return reinterpret_cast<T*>(this);
}

EventType KeyEvent::type() {
	return KeyEvent::TYPE;
}
KeyEvent::KeyEvent(Application* app, int key, int scancode, int action, int mods)
	: app(app), key(key), scancode(scancode), action(action), mods(mods) {}

EventType CursorEvent::type() {
	return CursorEvent::TYPE;
}
CursorEvent::CursorEvent(Application* app, double x, double y, double dx, double dy)
	: app(app), x(x), y(y), dx(dx), dy(dy) {}

EventType ScrollEvent::type() {
	return ScrollEvent::TYPE;
}
ScrollEvent::ScrollEvent(Application* app, double dx, double dy)
	: app(app), dx(dx), dy(dy) {}

EventType FramebufferSizeEvent::type() {
	return FramebufferSizeEvent::TYPE;
}
FramebufferSizeEvent::FramebufferSizeEvent(Application* app, int width, int height)
	: app(app), width(width), height(height) {}

EventType EnterChunkEvent::type() {
    return EnterChunkEvent::TYPE;
}

EnterChunkEvent::EnterChunkEvent(Application* app, int oldX, int oldZ, int newX, int newZ) : app(app), oldX(oldX), oldZ(oldZ), newX(newX), newZ(newZ) {}

template KeyEvent* Event::get<KeyEvent>();
template ScrollEvent* Event::get<ScrollEvent>();
template CursorEvent* Event::get<CursorEvent>();
template FramebufferSizeEvent* Event::get<FramebufferSizeEvent>();
template EnterChunkEvent* Event::get<EnterChunkEvent>();
