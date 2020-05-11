#include "../include/Widget.h"

Rectangle::Rectangle(int x, int y, int width, int height)
    : position(x, y), size(width, height) {}

Rectangle::Rectangle(glm::vec2 position, glm::vec2 size)
    : position(position), size(size) {}

Widget::Widget(const std::string& id, Layout* parent)
    : m_id(id), m_parent(parent) {}

void Widget::resize(const Rectangle& parent) {
    m_area.position = parent.position + m_properties.position * parent.size;
    m_area.size = m_properties.size * parent.size;
}

UiProperties& Widget::getProperties() {
    return m_properties;
}

const std::string& Widget::getId() const {
    return m_id;
}

Layout* Widget::getParent() const {
    return m_parent;
}

Rectangle Widget::getArea() const {
    return m_area;
}
