#pragma once

#include "UiProperties.h"

#include <map>
#include <string>

struct Rectangle {
    glm::vec2 position;
    glm::vec2 size;

    Rectangle(int x = 0, int y = 0, int width = 0, int height = 0);
    Rectangle(glm::vec2 position, glm::vec2 size);
};

class Layout;

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

    public:
        Widget(const std::string& id, Layout* parent);
        virtual ~Widget() = default;

        virtual void draw() const = 0;
        virtual void resize(const Rectangle& parent);

        UiProperties& getProperties();

        const std::string& getId() const;
        Layout *getParent() const;
        Rectangle getArea() const;
};
