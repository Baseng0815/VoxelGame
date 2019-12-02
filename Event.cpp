#include "pch.h"

#include "Event.h"

template<class T>
T* Event::get() {
	return reinterpret_cast<T*>(this);
}

EventType KeyEvent::type() {
	return KeyEvent::TYPE;
}
KeyEvent::KeyEvent(Application* app = nullptr, int key = 0, int scancode = 0, int action = 0, int mods = 0)
	: app(app), key(key), scancode(scancode), action(action), mods(mods) {}

EventType CursorEvent::type() {
	return CursorEvent::TYPE;
}
CursorEvent::CursorEvent(Application* app = nullptr, double x = 0, double y = 0, double dx = 0, double dy = 0)
	: app(app), x(x), y(y), dx(dx), dy(dy) {}

EventType ScrollEvent::type() {
	return ScrollEvent::TYPE;
}
ScrollEvent::ScrollEvent(Application* app = 0, double dx = 0, double dy = 0)
	: app(app), dx(dx), dy(dy) {}

EventType FramebufferSizeEvent::type() {
	return FramebufferSizeEvent::TYPE;
}
FramebufferSizeEvent::FramebufferSizeEvent(Application* app = nullptr, int width = 0, int height = 0)
	: app(app), width(width), height(height) {}