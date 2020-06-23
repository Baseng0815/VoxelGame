#pragma once

#include "UiProperties.h"
#include "../Rendering/RenderQuad.h"

#include <map>
#include <string>

class Layout;
class Shader;

class Widget {
    protected:
        // everything is in screen space with (0|0) at the bottom left corner
        Rectangle m_finalArea; // the final area the widget ends up taking, including padding
        Rectangle m_widgetArea; // the area the widget ends up taking, excluding padding

        friend class GUI;
        void applyPadding();
        float m_minWidth = 0, m_minHeight = 0; // the minimal area the widget needs

        UiProperties m_properties;
        std::string m_id;

        // these custom event handlers are invoked by the functions below
        virtual void _onMove(int x, int y);
        virtual void _onEnter(int x, int y);
        virtual void _onLeave(int x, int y);
        virtual void _onPress(int x, int y);
        virtual void _onRelease(int x, int y);

        // these event handlers are called by the GUI and invoke the EventDispatcher
        // as well as the custom event handlers
        void onMove(int x, int y, int dx, int dy);
        void onEnter(int x, int y);
        void onLeave(int x, int y);
        void onPress(int x, int y);
        void onRelease(int x, int y);
        bool m_isHovering = false, m_isPressed = false;

        // get current background color based on hovering and pressed
        Color getBackgroundColor() const;
        // get current foreground color based on hovering and pressed
        Color getForegroundColor() const;

        RenderQuad m_renderQuadBackground;
        Shader* m_coloredQuadShader;

        virtual void _draw(const glm::mat4& projection) const;

    public:
        Widget(const std::string& id);
        virtual ~Widget() = default;

        void draw(const glm::mat4& projection) const;

        // update the area which the widget occupies
        virtual void updateArea(const Rectangle& parent);

        // update on-screen elements based on the area, like
        // the background color quad
        virtual void updateScreenElements();

        UiProperties& getProperties();

        glm::vec2 getPosition() const;
        glm::vec2 getSize() const;
        void setPosition(glm::vec2 position);

        const std::string& getId() const;
        Layout *getParent() const;
        Rectangle getArea() const;
};
