#include "../../include/Gui/Widget.h"
#include "../../include/Gui/Layout.h"

#include "../../include/ResourceManager.h"
#include "../../include/Resources/Shader.h"

#include <algorithm>

Color Widget::getBackgroundColor() const {
    if (m_isPressed)
        return m_properties.pressBackground;
    else if (m_isHovering)
        return m_properties.hoverBackground;
    else
        return m_properties.backgroundColor;
}

Color Widget::getForegroundColor() const {
    if (m_isPressed)
        return m_properties.pressForeground;
    else if (m_isHovering)
        return m_properties.hoverForeground;
    else
        return m_properties.foregroundColor;
}

void Widget::applyPadding() {
    m_finalArea = m_widgetArea;
    m_finalArea.position.x -= m_properties.padding.left;
    m_finalArea.size.x += m_properties.padding.right + m_properties.padding.left;
    m_finalArea.position.y -= m_properties.padding.bottom;
    m_finalArea.size.y += m_properties.padding.top + m_properties.padding.bottom;
}

void Widget::_draw(const glm::mat4& projection) const {
    // default widget does nothing special
}

Widget::Widget(const std::string& id, GUI *gui)
    : m_id(id), m_gui(gui) {
    m_coloredQuadShader = ResourceManager::getResource<Shader>("shaderColoredQuad");
}

void Widget::draw(const glm::mat4& projection) const {
    if (m_properties.isVisible) {
        // draw background
        m_coloredQuadShader->bind();
        m_coloredQuadShader->upload("color", m_properties.backgroundColor);
        m_coloredQuadShader->upload("projectionMatrix", projection);
        m_renderQuadBackground.render();

        // draw custom
        _draw(projection);
    }
}

#include <iostream>
void Widget::updateArea(const Rectangle& parent) {
    m_widgetArea = m_properties.constraints.getRect(parent, m_properties, m_minWidth, m_minHeight);
    applyPadding();
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

    // readjust widget area
    m_widgetArea.position.x = m_finalArea.position.x + m_properties.padding.left;
    m_widgetArea.position.y = m_finalArea.position.y + m_properties.padding.bottom;
    m_widgetArea.size.x = m_finalArea.size.x - m_properties.padding.right;
    m_widgetArea.size.y = m_finalArea.size.y - m_properties.padding.top;
}

const std::string& Widget::getId() const {
    return m_id;
}

Rectangle Widget::getArea() const {
    return m_finalArea;
}
