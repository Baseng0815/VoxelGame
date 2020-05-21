#pragma once

#include "UiProperties.h"
#include "RenderQuad.h"

#include <map>
#include <string>

class Layout;
class Shader;

class Widget {
    protected:
        // m_area is in screen space
        mutable Rectangle m_area;
        Layout* m_parent;

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
        Widget(const std::string& id, Layout* parent);
        virtual ~Widget() = default;

        void draw(Shader& shader) const;
        // parent argument only relevant to root layout
        virtual void resize(Rectangle parent = Rectangle());

        UiProperties& getProperties();

        glm::vec2 getPosition() const;
        glm::vec2 getSize() const;

        const std::string& getId() const;
        Layout *getParent() const;
        Rectangle getArea() const;
};
