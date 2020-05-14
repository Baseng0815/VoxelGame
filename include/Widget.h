#pragma once

#include "UiProperties.h"
#include "RenderQuad.h"

#include <map>
#include <string>

class Layout;
class Shader;

class Widget {
    protected:
        // m_area is in global space (0|0-1|1)
        Rectangle m_area;
        Layout* m_parent;

        UiProperties m_properties;

        std::string m_id;

        // TODO move to event dispatcher
        void onHover();
        void onLeave();
        void onPress();
        void onRelease();

        RenderQuad m_renderQuad;
        virtual void _draw(Shader& shader) const;

    public:
        Widget(const std::string& id, Layout* parent);
        virtual ~Widget() = default;

        void draw(Shader& shader) const;
        virtual void resize();

        UiProperties* getProperties();

        const std::string& getId() const;
        Layout *getParent() const;
        Rectangle getArea() const;
};
