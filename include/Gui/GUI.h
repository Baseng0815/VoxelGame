#pragma once

#include "Layout.h"

struct FramebufferSizeEvent;
struct CursorEvent;
struct MouseButtonEvent;
struct KeyEvent;

class Shader;
class CameraComponent;

typedef unsigned int CallbackId;

class GUI {
    protected:
        // contains all widgets which are currently alive
        std::map<std::string, Widget*> m_widgets;

        bool coordinatesInWidget(const Widget& widget, int x, int y);

        void handleFramebufferSize(const FramebufferSizeEvent&);
        void handleCursorMove(const CursorEvent&);
        void handleButtonPress(const MouseButtonEvent&);
        void handleKeyPress(const KeyEvent&);

        glm::mat4 m_orthoProjection;

        // TODO improve performance by moving outdated flag into widgets
        Layout* m_rootLayout;
        bool m_isOutdated = true;

    public:
        GUI();
        ~GUI();

        void draw();
        void invalidate();
        void update();

        // widgets register and unregister themselves
        void registerWidget(Widget* widget);

        Widget& getWidget(const std::string& id);
};
