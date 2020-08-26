#pragma once

#include "Layout.h"

#include <stack>

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

        static bool coordinatesInWidget(const Widget& widget, int x, int y);

        void handleFramebufferSize(const FramebufferSizeEvent&);
        void handleCursorMove(const CursorEvent&);
        void handleButtonPress(const MouseButtonEvent&);
        void handleKeyPress(const KeyEvent&);

        glm::mat4 m_orthoProjection;
        bool m_isOutdated = true;

        std::stack<Layout*> m_rootLayouts;

    public:
        GUI();
        ~GUI();

        void draw();
        void update();

        Widget& getWidget(const std::string& id);

        template<typename T>
            T& getWidget(const std::string& id) {
                return static_cast<T&>(getWidget(id));
            }

        // TODO add pushPanel and popPanel which disables underlying panels as stack
        // a panel is basically a layout with hard-coded widgets
        void addPanel(Layout* layout);

        // widgets register and unregister themselves
        void __registerWidget(Widget* widget);
        void __invalidate();
};
