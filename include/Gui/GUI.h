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

        CallbackHandle<const FramebufferSizeEvent&> m_framebufferSizeHandle;
        void handleFramebufferSize(const FramebufferSizeEvent&);

        glm::mat4 m_orthoProjection;

        std::vector<Layout*> m_rootLayouts;

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
};
