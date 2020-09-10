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

class Application;
struct Block;

struct KeyEvent {
    Application* app;
    int key;
    int scancode;
    int action;
    int mods;

    std::string toString() const;
};

struct MouseButtonEvent {
    Application* app;
    int button;
    int action;
    int mods;

    std::string toString() const;
};

struct CursorEvent {
    Application* app;
    double x, y;
    double dx, dy;

    std::string toString() const;
};

struct ScrollEvent {
    Application* app;
    double dx, dy;

    std::string toString() const;
};

struct FramebufferSizeEvent {
    Application* app;
    float width, height;

    std::string toString() const;
};

struct EnterChunkEvent {
    Application* app;
    int oldX, oldZ;
    int newX, newZ;

    std::string toString() const;
};

struct BlockChangedEvent {
    Application* app;
    glm::vec3 position;

    std::string toString() const;
};

struct EntityMovedEvent {
    Application* app;
    entt::entity entity;
    glm::vec3 newPos, oldPos;

    std::string toString() const;
};
