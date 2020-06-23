#pragma once

#include <sstream>
#include <string.h>
#include <iostream>

#include <glm/glm.hpp>
#include <entt/entt.hpp>

enum EventType {
    // basic events
    KEY_EVENT, MOUSE_BUTTON_EVENT, CURSOR_EVENT, SCROLL_EVENT, FRAMEBUFFER_SIZE_EVENT,

    // advanced events
    ENTER_CHUNK_EVENT, BLOCK_CHANGED_EVENT, ENTITY_MOVED_EVENT
};

using namespace std::placeholders;

class Application;
struct Block;

struct Event {
    virtual EventType type() const = 0;
    virtual std::string toString() const = 0;


    template<class T>
        T* get() {
            return reinterpret_cast<T*>(this);
        }

    friend std::ostream& operator<<(std::ostream& ostream, const Event& event) {
        ostream << event.toString();
        return ostream;
    }
};

struct KeyEvent : public Event {
    static constexpr EventType TYPE = KEY_EVENT;

    Application* app;
    int key;
    int scancode;
    int action;
    int mods;

    EventType type() const override final {
        return KeyEvent::TYPE;
    }
    std::string toString() const override final {
        std::stringstream ss;
        ss << "key: " << key << " scancode: " << scancode << " action: " << action << " mods: " << mods;
        return ss.str();
    }
    KeyEvent(Application* app = nullptr, int key = 0, int scancode = 0, int action = 0, int mods = 0)
        : app(app), key(key), scancode(scancode), action(action), mods(mods) {}
};

struct MouseButtonEvent : public Event {
    static constexpr EventType TYPE = MOUSE_BUTTON_EVENT;

    Application* app;
    int button;
    int action;
    int mods;

    EventType type() const override final {
        return MouseButtonEvent::TYPE;
    }
    std::string toString() const override final {
        std::stringstream ss;
        ss << "button: " << button << " action: " << action << " mods: " << mods;
        return ss.str();
    }
    MouseButtonEvent(Application* app = nullptr, int button = 0, int action = 0, int mods = 0)
        : app(app), button(button), action(action), mods(mods) {}
};

struct CursorEvent : public Event {
    static constexpr EventType TYPE = CURSOR_EVENT;

    Application* app;
    double x, y;
    double dx, dy;

    EventType type() const override final {
        return CursorEvent::TYPE;
    }
    std::string toString() const override final {
        std::stringstream ss;
        ss << "x: " << x << " y: " << y << " dx: " << dx << " dy: " << dy;
        return ss.str();
    }
    CursorEvent(Application* app = nullptr, double x = 0, double y = 0, double dx = 0, double dy = 0)
        : app(app), x(x), y(y), dx(dx), dy(dy) {}
};

struct ScrollEvent : public Event {
    static constexpr EventType TYPE = SCROLL_EVENT;

    Application* app;
    double dx, dy;

    EventType type() const override final {
        return ScrollEvent::TYPE;
    }
    std::string toString() const override final {
        std::stringstream ss;
        ss << "dx: " << dx << " dy: " << dy;
        return ss.str();
    }
    ScrollEvent(Application* app = nullptr, double dx = 0, double dy = 0)
        : app(app), dx(dx), dy(dy) {}
};

struct FramebufferSizeEvent : public Event {
    static constexpr EventType TYPE = FRAMEBUFFER_SIZE_EVENT;

    Application* app;
    float width, height;

    EventType type() const override final {
        return FramebufferSizeEvent::TYPE;
    }
    std::string toString() const override final {
        std::stringstream ss;
        ss << "width: " << width << " height: " << height;
        return ss.str();
    }
    FramebufferSizeEvent(Application* app = nullptr, float width = 0, float height = 0)
        : app(app), width(width), height(height) {}
};

struct EnterChunkEvent : public Event {
    static constexpr EventType TYPE = ENTER_CHUNK_EVENT;

    Application* app;
    int oldX, oldZ;
    int newX, newZ;

    EventType type() const override final {
        return EnterChunkEvent::TYPE;
    }
    std::string toString() const override final {
        std::stringstream ss;
        ss << "oldX: " << oldX << " oldZ: " << oldZ << " newX: " << newX << " newZ: " << newZ;
        return ss.str();
    }
    EnterChunkEvent(Application* app = nullptr, int oldX = 0, int oldY = 0, int newX = 0, int newY = 0);
};

struct BlockChangedEvent : public Event {
    static constexpr EventType TYPE = BLOCK_CHANGED_EVENT;

    Application* app;
    glm::vec3 position;

    EventType type() const override final {
        return BlockChangedEvent::TYPE;
    }
    std::string toString() const override final {
        std::stringstream ss;
        ss << "x: " << position.x << " y: " << position.y << " z: " << position.z;
        return ss.str();
    }
    BlockChangedEvent(Application* app = nullptr, glm::vec3 position = glm::vec3())
        : app(app), position(position) {}
};

struct EntityMovedEvent : public Event {
    static constexpr EventType TYPE = ENTITY_MOVED_EVENT;

    Application* app;
    entt::entity entity;
    glm::vec3 newPos, oldPos;

    EventType type() const override final {
        return EntityMovedEvent::TYPE;
    }
    std::string toString() const override final {
        std::stringstream ss;
        ss << "x: " << oldPos.x << " y: " << oldPos.y << " z: " << oldPos.z << " -> ";
        ss << "x: " << newPos.x << " y: " << newPos.y << " z: " << newPos.z;
        return ss.str();
    }
    EntityMovedEvent(Application* app = nullptr, entt::entity entity = entt::entity(), glm::vec3 oldPos = glm::vec3(), glm::vec3 newPos = glm::vec3())
        : app(app), entity(entity), newPos(newPos), oldPos(oldPos) {}
};
