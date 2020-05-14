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
    shader.upload("blendFactor", 0);
    m_renderQuad.render();

    // draw custom
    shader.upload("blendFactor", 1);
    _draw(shader);
}

void Widget::resize() {
    // clamp position and cut off if widget overlaps into others
    m_properties.position.x = std::clamp(m_properties.position.x, 0.f, 1.f);
    m_properties.position.x = std::clamp(m_properties.position.x, 0.f, 1.f);

    m_properties.size.x = std::min(m_properties.size.x, 1 - m_properties.position.x);
    m_properties.size.y = std::min(m_properties.size.y, 1 - m_properties.position.y);

    // transform into global space and apply to area
    UiProperties* propertiesParent = m_parent->getProperties();
    m_area.position = propertiesParent->position + m_properties.position * propertiesParent->size;
    m_area.size = m_properties.size * propertiesParent->size;

    m_renderQuad.resize(m_area);
}

UiProperties* Widget::getProperties() {
    return &m_properties;
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
