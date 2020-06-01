#pragma once

#include "UiProperties.h"
#include "RenderQuad.h"

#include <map>
#include <string>

class Layout;
class Shader;

class Widget {
    protected:
        // everything is in screen space
        Rectangle m_finalArea; // the final area the widget ends up taking
        float m_minWidth = 0, m_minHeight = 0; // the minimal area the widget needs

        UiProperties m_properties;

        std::string m_id;

        // TODO move to event dispatcher
        void onHover();
        void onLeave();
        void onPress();
        void onRelease();

        RenderQuad m_renderQuadBackground;
        virtual void _draw(Shader& shader) const;

    public:
        Widget(const std::string& id);
        virtual ~Widget() = default;

        void draw(Shader& shader) const;

        // updates the area which the widget occupies
        virtual void updateArea(const Rectangle& parent);

        // updates on-screen elements based on the area, like
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
