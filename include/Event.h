#pragma once

#include <string.h>
#include <iostream>
#include <functional>

#include <glm/glm.hpp>
#include <entt/entt.hpp>

using namespace std::placeholders;

class Application;
struct Block;

typedef unsigned int CallbackId;

enum EventType {
    // basic events
    KEY_EVENT, MOUSE_BUTTON_EVENT, CURSOR_EVENT, SCROLL_EVENT, FRAMEBUFFER_SIZE_EVENT,

    // advanced events
    ENTER_CHUNK_EVENT, BLOCK_CHANGED_EVENT, ENTITY_MOVED_EVENT
};

struct Event {
    virtual EventType type() const = 0;
    virtual std::string toString() const = 0;


    template<class T>
        T* get();

    friend std::ostream& operator<<(std::ostream& ostream, const Event& event);
};

typedef std::function<void(Event*)> Callback;

struct KeyEvent : public Event {
    static constexpr EventType TYPE = KEY_EVENT;

    Application* app;
    int key;
    int scancode;
    int action;
    int mods;

    EventType type() const override final;
    std::string toString() const override final;
    KeyEvent(Application* app = nullptr, int key = 0, int scancode = 0, int action = 0, int mods = 0);
};

struct MouseButtonEvent : public Event {
    static constexpr EventType TYPE = MOUSE_BUTTON_EVENT;

    Application* app;
    int button;
    int action;
    int mods;

    EventType type() const override final;
    std::string toString() const override final;
    MouseButtonEvent(Application* app = nullptr, int button = 0, int action = 0, int mods = 0);
};

struct CursorEvent : public Event {
    static constexpr EventType TYPE = CURSOR_EVENT;

    Application* app;
    double x, y;
    double dx, dy;

    EventType type() const override final;
    std::string toString() const override final;
    CursorEvent(Application* app = nullptr, double x = 0, double y = 0, double dx = 0, double dy = 0);
};

struct ScrollEvent : public Event {
    static constexpr EventType TYPE = SCROLL_EVENT;

    Application* app;
    double dx, dy;

    EventType type() const override final;
    std::string toString() const override final;
    ScrollEvent(Application* app = nullptr, double dx = 0, double dy = 0);
};

struct FramebufferSizeEvent : public Event {
    static constexpr EventType TYPE = FRAMEBUFFER_SIZE_EVENT;

    Application* app;
    float width, height;

    EventType type() const override final;
    std::string toString() const override final;
    FramebufferSizeEvent(Application* app = nullptr, float width = 0, float height = 0);
};

struct EnterChunkEvent : public Event {
    static constexpr EventType TYPE = ENTER_CHUNK_EVENT;

    Application* app;
    int oldX, oldZ;
    int newX, newZ;

    EventType type() const override final;
    std::string toString() const override final;
    EnterChunkEvent(Application* app = nullptr, int oldX = 0, int oldY = 0, int newX = 0, int newY = 0);
};


struct BlockChangedEvent : public Event {
    static constexpr EventType TYPE = BLOCK_CHANGED_EVENT;

    Application* app;
    glm::vec3 position;

    EventType type() const override final;
    std::string toString() const override final;
    BlockChangedEvent(Application* app = nullptr, glm::vec3 position = glm::vec3());
};

struct EntityMovedEvent : public Event {
    static constexpr EventType TYPE = ENTITY_MOVED_EVENT;

    Application* app;
    entt::entity entity;
    glm::vec3 newPos, oldPos;

    EventType type() const override final;
    std::string toString() const override final;
    EntityMovedEvent(Application* app = nullptr, entt::entity entity = entt::entity(), glm::vec3 oldPos = glm::vec3(), glm::vec3 newPos = glm::vec3());
};
