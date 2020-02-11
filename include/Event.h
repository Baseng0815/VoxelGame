#pragma once

#include <functional>

using namespace std::placeholders;

typedef unsigned int CallbackId;
class Application;

enum EventType {
    // basic events
    KEY_EVENT, CURSOR_EVENT, SCROLL_EVENT, FRAMEBUFFER_SIZE_EVENT,

    // advanced events
    ENTER_CHUNK_EVENT
};

struct Event {
    virtual EventType type() = 0;

    template<class T>
        T* get();
};

typedef std::function<void(Event *e)> Callback;

struct KeyEvent : public Event {
    static constexpr EventType TYPE = KEY_EVENT;

    Application* app;
    int key;
    int scancode;
    int action;
    int mods;

    EventType type() override final;
    KeyEvent(Application* app = nullptr, int key = 0, int scancode = 0, int action = 0, int mods = 0);
};

struct CursorEvent : public Event {
    static constexpr EventType TYPE = CURSOR_EVENT;

    Application* app;
    double x, y;
    double dx, dy;

    EventType type() override final;
    CursorEvent(Application* app = nullptr, double x = 0, double y = 0, double dx = 0, double dy = 0);
};

struct ScrollEvent : public Event {
    static constexpr EventType TYPE = SCROLL_EVENT;

    Application* app;
    double dx, dy;

    EventType type() override final;
    ScrollEvent(Application* app = nullptr, double dx = 0, double dy = 0);
};

struct FramebufferSizeEvent : public Event {
    static constexpr EventType TYPE = FRAMEBUFFER_SIZE_EVENT;

    Application* app;
    int width, height;

    EventType type() override final;
    FramebufferSizeEvent(Application* app = nullptr, int width = 0, int height = 0);
};

struct EnterChunkEvent : public Event {
    static constexpr EventType TYPE = ENTER_CHUNK_EVENT;

    Application* app;
    int oldX, oldZ;
    int newX, newZ;

    EventType type() override final;
    EnterChunkEvent(Application* app = nullptr, int oldX = 0, int oldY = 0, int newX = 0, int newY = 0);
};