#include "../../include/Gui/Widget.h"
#include "../../include/Gui/Layout.h"

#include "../../include/ResourceManager.h"
#include "../../include/Resources/Shader.h"

#include <algorithm>

Color Widget::getBackgroundColor() const
{
    if (m_isPressed)
        return m_properties.pressBackground;
    else if (m_isHovering)
        return m_properties.hoverBackground;
    else
        return m_properties.backgroundColor;
}

Color Widget::getForegroundColor() const
{
    if (m_isPressed)
        return m_properties.pressForeground;
    else if (m_isHovering)
        return m_properties.hoverForeground;
    else
        return m_properties.foregroundColor;
}

void Widget::applyPadding()
{
    m_outerArea = m_innerArea;
    m_outerArea.position.x -= m_properties.padding.left;
    m_outerArea.size.x += m_properties.padding.right + m_properties.padding.left;
    m_outerArea.position.y -= m_properties.padding.bottom;
    m_outerArea.size.y += m_properties.padding.top + m_properties.padding.bottom;
}

void Widget::_draw(const glm::mat4& projection) const
{
    // default widget does nothing special
}

void Widget::_updateScreenElements()
{
    // default widget does nothing special
}

void Widget::_updateMinimumSize()
{
    // default widget does nothing special
}

Widget::Widget(const std::string& id)
    : m_id {id}, m_coloredQuadShader {ResourceManager::getResource<Shader>("shaderColoredQuad")}
{}

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

void Widget::updateArea(const Rectangle& parent)
{
    _updateMinimumSize();

    m_innerArea = m_properties.constraints.getRect(parent, m_properties, m_minWidth, m_minHeight);
    applyPadding();

    _updateArea();
}

void Widget::updateScreenElements()
{
    m_renderQuadBackground.resize(m_outerArea);
    _updateScreenElements();
}

UiProperties &Widget::properties()
{ return m_properties; }

const glm::vec2 &Widget::getPosition() const
{ return m_outerArea.position; }

const glm::vec2 &Widget::getSize() const
{ return m_outerArea.size; }

const Rectangle &Widget::getInnerArea() const
{ return m_innerArea; }
const Rectangle &Widget::getOuterArea() const
{ return m_outerArea; }

void Widget::setPosition(const glm::vec2 &position)
{
    m_outerArea.position = position;

    // readjust inner positions
    m_innerArea.position.x = m_outerArea.position.x + m_properties.padding.left;
    m_innerArea.position.y = m_outerArea.position.y + m_properties.padding.bottom;
}

void Widget::setSize(const glm::vec2 &size)
{
    m_outerArea.size = size;

    // readjust inner sizes
    m_innerArea.size.x = m_outerArea.size.y - m_properties.padding.left - m_properties.padding.right;
    m_innerArea.size.y = m_outerArea.size.y - m_properties.padding.top - m_properties.padding.bottom;
}

bool Widget::isHovering() const
{ return m_isHovering };
bool Widget::isPressed() const
{ return m_isPressed; }

const std::string& Widget::getId() const {
    return m_id;
}
