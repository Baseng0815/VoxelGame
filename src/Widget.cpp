#include "../include/Widget.h"

#include "../include/Shader.h"
#include "../include/Layout.h"

#include <algorithm>

void Widget::_draw(Shader& shader) const {
    // default widget does nothing special
}

Widget::Widget(const std::string& id)
    : m_id(id) {}

void Widget::draw(Shader& shader) const {
    // draw background
    shader.upload("color", m_properties.backgroundColor);
    shader.upload("textureVisibility", 0.f);
    m_renderQuadBackground.render();

    // draw custom
    shader.upload("textureVisibility", 1.f);
    _draw(shader);
}

void Widget::updateArea(const Rectangle& parent) {
    m_finalArea = m_properties.constraints.getRect(parent, m_minWidth, m_minHeight);
}

void Widget::updateScreenElements() {
    m_renderQuadBackground.resize(m_finalArea);
}

UiProperties& Widget::getProperties() {
    return m_properties;
}

glm::vec2 Widget::getPosition() const {
    return m_finalArea.position;
}

glm::vec2 Widget::getSize() const {
    return m_finalArea.size;
}

void Widget::setPosition(glm::vec2 position) {
    m_finalArea.position = position;
}

const std::string& Widget::getId() const {
    return m_id;
}

Rectangle Widget::getArea() const {
    return m_finalArea;
}
