#include "../../include/Events/Event.hpp"
#include "../../include/Math/Cuboid.hpp"

#include "../../include/Utility.hpp"
#include "../../include/WorldGeneration/Structures.hpp"

std::string KeyEvent::toString() const {
    std::stringstream ss;
    ss << "key: " << key << " scancode: " << scancode << " action: " << action << " mods: " << mods;
    return ss.str();
}

std::string MouseButtonEvent::toString() const {
    std::stringstream ss;
    ss << "button: " << button << " action: " << action << " mods: " << mods;
    return ss.str();
}

std::string CursorEvent::toString() const {
    std::stringstream ss;
    ss << "x: " << x << " y: " << y << " dx: " << dx << " dy: " << dy;
    return ss.str();
}

std::string ScrollEvent::toString() const {
    std::stringstream ss;
    ss << "dx: " << dx << " dy: " << dy;
    return ss.str();
}

std::string FramebufferSizeEvent::toString() const {
    std::stringstream ss;
    ss << "width: " << width << " height: " << height;
    return ss.str();
}

std::string EnterChunkEvent::toString() const {
    std::stringstream ss;
    ss << "oldX: " << oldX << " oldZ: " << oldZ << " newX: " << newX << " newZ: " << newZ;
    return ss.str();
}

std::string BlockChangedEvent::toString() const {
    std::stringstream ss;
    ss << "x: " << position.x << " y: " << position.y << " z: " << position.z;
    return ss.str();
}

std::string EntityMovedEvent::toString() const {
    std::stringstream ss;
    ss << oldPos << " -> ";
    ss << newPos;
    return ss.str();
}

std::string BlockCollisionEvent::toString() const {
    std::stringstream ss;
    ss << "entity: " << entt::to_integral(entity);
    ss << "block: " << block;
    return ss.str();
}

