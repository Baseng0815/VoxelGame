#include "../include/Widget.h"

#include "../include/Shader.h"
#include "../include/Layout.h"

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
    auto& propertiesParent = m_parent->getProperties();
    m_area.position = propertiesParent.position + m_properties.position * propertiesParent.size;
    m_area.size = m_properties.size * propertiesParent.size;
    m_renderQuad = RenderQuad(m_area);
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
