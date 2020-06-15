#pragma once

#include "Layout.h"

struct Event;
class Shader;
class CameraComponent;

typedef unsigned int CallbackId;

class GUI {
    protected:
        // contains all widgets which are currently alive
        std::map<std::string, Widget*> m_widgets;

        std::vector<CallbackId> m_callbackIds;
        void handleFramebufferSize(Event* event);
        void handleCursorMove(Event* e);
        void handleButtonPress(Event* e);
        void handleKeyPress(Event* e);

        Shader* m_guiShader;
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
};
