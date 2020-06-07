#pragma once

#include <functional>

enum EventType {
    // basic events
    KEY_EVENT, MOUSE_BUTTON_EVENT, CURSOR_EVENT, SCROLL_EVENT, FRAMEBUFFER_SIZE_EVENT,

    // advanced events
    ENTER_CHUNK_EVENT, BLOCK_CHANGED_EVENT, ENTITY_MOVED_EVENT
};

class Event;

typedef unsigned int CallbackId;
typedef std::function<void(Event*)> CallbackFun;

struct Callback {
    CallbackId id;
    EventType type;
    bool isActive = true;
    CallbackFun fun;

    Callback(CallbackFun fun, EventType type, bool isActive = true);

    bool operator==(const CallbackId& id);
};
