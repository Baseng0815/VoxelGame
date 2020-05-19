#include "../include/Event.h"
#include "../include/Block.h"

#include <sstream>

template<class T>
T* Event::get() {
    return reinterpret_cast<T*>(this);
}

std::ostream& operator<<(std::ostream& ostream, const Event& event) {
    ostream << event.toString();
    return ostream;
}

EventType KeyEvent::type() const {
	return KeyEvent::TYPE;
}

std::string KeyEvent::toString() const {
    std::stringstream ss;
    ss << "key: " << key << " scancode: " << scancode << " action: " << action << " mods: " << mods;
    return ss.str();
}

KeyEvent::KeyEvent(Application* app, int key, int scancode, int action, int mods)
	: app(app), key(key), scancode(scancode), action(action), mods(mods) {}

EventType MouseButtonEvent::type() const {
    return MouseButtonEvent::TYPE;
}

std::string MouseButtonEvent::toString() const {
    std::stringstream ss;
    ss << "button: " << button << " action: " << action << " mods: " << mods;
    return ss.str();
}

MouseButtonEvent::MouseButtonEvent(Application* app, int button, int action, int mods)
    : app(app), button(button), action(action), mods(mods) {}

EventType CursorEvent::type() const {
	return CursorEvent::TYPE;
}

std::string CursorEvent::toString() const {
    std::stringstream ss;
    ss << "x: " << x << " y: " << y << " dx: " << dx << " dy: " << dy;
    return ss.str();
}

CursorEvent::CursorEvent(Application* app, double x, double y, double dx, double dy)
	: app(app), x(x), y(y), dx(dx), dy(dy) {}

EventType ScrollEvent::type() const {
	return ScrollEvent::TYPE;
}

std::string ScrollEvent::toString() const {
    std::stringstream ss;
    ss << "dx: " << dx << " dy: " << dy;
    return ss.str();
}

ScrollEvent::ScrollEvent(Application* app, double dx, double dy)
	: app(app), dx(dx), dy(dy) {}

EventType FramebufferSizeEvent::type() const {
	return FramebufferSizeEvent::TYPE;
}

std::string FramebufferSizeEvent::toString() const {
    std::stringstream ss;
    ss << "width: " << width << " height: " << height;
    return ss.str();
}

FramebufferSizeEvent::FramebufferSizeEvent(Application* app, int width, int height)
	: app(app), width(width), height(height) {}

EventType EnterChunkEvent::type() const {
    return EnterChunkEvent::TYPE;
}

EnterChunkEvent::EnterChunkEvent(Application* app, int oldX, int oldZ, int newX, int newZ) : app(app), oldX(oldX), oldZ(oldZ), newX(newX), newZ(newZ) {}

std::string EnterChunkEvent::toString() const {
    std::stringstream ss;
    ss << "oldX: " << oldX << " oldZ: " << oldZ << " newX: " << newX << " newZ: " << newZ;
    return ss.str();
}


EventType BlockChangedEvent::type() const {
    return BlockChangedEvent::TYPE;
}

BlockChangedEvent::BlockChangedEvent(Application* app, glm::ivec3 position) : app(app), position(position) {}

std::string BlockChangedEvent::toString() const {
    std::stringstream ss;
    ss << "x: " << position.x << " y: " << position.y << " z: " << position.z;
    return ss.str();
}

//EntityMovedEvent::EntityMovedEvent(Application* app, entt::entity entity, glm::vec3 oldPos, glm::vec3 newPos)
//    : app(app), entity(entity), oldPos(oldPos), newPos(newPos) { }
//
//std::string EntityMovedEvent::toString() const {
//    std::stringstream ss;
//    ss << "x: " << oldPos.x << " y: " << oldPos.y << " z: " << oldPos.z << " -> ";
//    ss << "x: " << newPos.x << " y: " << newPos.y << " z: " << newPos.z;
//    return ss.str();
//}
//
//EventType EntityMovedEvent::type() const {
//    return EntityMovedEvent::TYPE;
//}

template KeyEvent* Event::get<KeyEvent>();
template MouseButtonEvent* Event::get<MouseButtonEvent>();
template ScrollEvent* Event::get<ScrollEvent>();
template CursorEvent* Event::get<CursorEvent>();
template FramebufferSizeEvent* Event::get<FramebufferSizeEvent>();
template EnterChunkEvent* Event::get<EnterChunkEvent>();
template BlockChangedEvent* Event::get<BlockChangedEvent>();
//template EntityMovedEvent* Event::get<EntityMovedEvent>();
