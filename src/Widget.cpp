#include "../include/Widget.h"

#include "../include/Shader.h"
#include "../include/Layout.h"

#include <algorithm>

void Widget::_draw(Shader& shader) const {
    // default widget does nothing special
}

Widget::Widget(const std::string& id, Layout* parent)
    : m_id(id), m_parent(parent) {}

void Widget::draw(Shader& shader) const {
    // draw background
    shader.upload("color", m_properties.backgroundColor);
    shader.upload("textureVisibility", 0.f);
    m_renderQuadBackground.render();

    // draw custom
    shader.upload("textureVisibility", 1.f);
    _draw(shader);
}

void Widget::updateArea(Rectangle parent) {
    m_area = m_properties.constraints.getRect(m_parent->getArea());
}

void Widget::updateScreenElements() {
    m_renderQuadBackground.resize(m_area);
}

UiProperties& Widget::getProperties() {
    return m_properties;
}

glm::vec2 Widget::getPosition() const {
    return m_area.position;
}

glm::vec2 Widget::getSize() const {
    return m_area.size;
}

void Widget::setPosition(glm::vec2 position) {
    m_area.position = position;
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
