#pragma once

#include "Layout.h"

class Event;
class Shader;
class CameraComponent;

class GUI {
    private:
        // contains all widgets which are currently alive
        std::map<std::string, Widget*> m_widgets;

        void handleFramebufferSize(Event* event);
        void handleCursorMove(Event* e);
        void handleButtonPress(Event* e);
        void handleKeyPress(Event* e);

        Shader* m_guiShader;

    public:
        GUI();
        ~GUI();

        void draw(const CameraComponent& camera);

        // register gui widget
        // should not be called manually
        void registerWidget(Widget* widget);
};
