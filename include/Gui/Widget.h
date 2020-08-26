#pragma once

#include "UiProperties.h"
#include "../Rendering/RenderQuad.h"

#include "../Events/CallbackList.h"

#include <map>
#include <string>

class GUI;
class Layout;
class Shader;

struct CursorEvent;
struct MouseButtonEvent;

class Widget {
    protected:
        // everything is in screen space with (0|0) at the bottom left corner
        Rectangle m_innerArea; // the area the widget takes up
        Rectangle m_outerArea; // the area the widget takes up plus padding

        // calculates outerArea using padding and innerArea
        void applyPadding();

        // the minimal area the widget needs
        // used for MATCH_CONSTRAINT
        glm::vec2 m_minSize;

        UiProperties m_properties;
        std::string m_id;

        bool m_isHovering = false, m_isPressed = false;

        // get current background color based on hovering and pressed
        Color getBackgroundColor() const;
        // get current foreground color based on hovering and pressed
        Color getForegroundColor() const;

        RenderQuad m_renderQuadBackground;
        const Shader* m_coloredQuadShader;

        virtual void _draw(const glm::mat4 &projection) const;
        virtual void _updateScreenElements();
        virtual void _updateMinimumSize();
        virtual void _updateArea();

    public:
        CallbackList<const CursorEvent&> onMove;
        CallbackList<const CursorEvent&> onEnter;
        CallbackList<const CursorEvent&> onLeave;
        CallbackList<const MouseButtonEvent&> onPress;
        CallbackList<const MouseButtonEvent&> onRelease;

        Widget(const std::string &id);
        virtual ~Widget() = default;

        void draw(const glm::mat4& projection) const;

        // update the area which the widget occupies
        void updateArea(const Rectangle &parent);

        // update on-screen elements based on the area
        void updateScreenElements();

        UiProperties &properties();

        // used in the layout step
        // always sets the outer area
        const glm::vec2 &getPosition() const;
        const glm::vec2 &getSize() const;
        const Rectangle &getInnerArea() const;
        const Rectangle &getOuterArea() const;
        void setPosition(const glm::vec2 &position);
        void setSize(const glm::vec2 &size);

        bool isHovering() const;
        bool isPressed() const;

        const std::string& getId() const;
        Layout *getParent() const;
};
