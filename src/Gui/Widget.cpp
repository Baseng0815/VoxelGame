//#include "../../include/Gui/Widget.hpp"
#include "../../include/Gui/Layout.hpp"

#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Shader.hpp"

void Widget::applyPadding() {
    m_innerArea = m_outerArea;
    m_innerArea.position.x += m_properties.padding.left;
    m_innerArea.size.x -= m_properties.padding.right + m_properties.padding.left;
    m_innerArea.position.y += m_properties.padding.top;
    m_innerArea.size.y -= m_properties.padding.top + m_properties.padding.bottom;
}

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

bool Widget::coordinatesInWidget(int x, int y) {
    if (x > m_outerArea.position.x && x < m_outerArea.position.x + m_outerArea.size.x &&
        y > m_outerArea.position.y && y < m_outerArea.position.y + m_outerArea.size.y) {
        return true;
    }
    else {
        return false;
    }
}

void Widget::handleCursorMove(const CursorEvent& e) {
    if (!m_properties.receiveEvents)
        return;

    // cursor y position is inverted in GUI space
    double screenY = EventDispatcher::getFramebufferSize().y - e.y;
    bool inArea = coordinatesInWidget(e.x, screenY);

    CursorEvent newE = e;
    newE.y = screenY;
    if (m_isHovering) {
        // cursor was on widget and now is not (leave)
        if (!inArea) {
            onLeave.invoke(newE);
            m_isHovering = false;
            // cursor was on widget and is still (move)
        }
        else {
            onMove.invoke(newE);
        }
    }
    else {
        // cursor was not on widget and is now (enter)
        if (inArea) {
            onEnter.invoke(newE);
            m_isHovering = true;
        }
    }
}

void Widget::handleButtonPress(const MouseButtonEvent& e) {
    if (!m_properties.receiveEvents)
        return;
    if (!m_isHovering)
        return;

    if (e.action == GLFW_PRESS) {
        onPress.invoke(e);
        m_isPressed = true;
    }
    else if (e.action == GLFW_RELEASE) {
        onRelease.invoke(e);
        m_isPressed = false;
    }
}

void Widget::_draw(const glm::mat4& projection) const {
    // default widget does nothing special
}

void Widget::_updateScreenElements() {
    // default widget does nothing special
}

void Widget::_updateMinimumSize() {
    // default widget does nothing special
}

void Widget::_updateArea() {
    // default widget does nothing special
}

Widget::Widget(const std::string& id)
    : m_id{id}, m_coloredQuadShader{ResourceManager::getResource<Shader>(SHADER_COLOR_QUAD)} {
    m_cursorCallbackHandle = EventDispatcher::onCursorMove.subscribe([&](const CursorEvent& e) {
        handleCursorMove(e);
    });

    m_mouseButtonCallbackHandle = EventDispatcher::onMouseButtonPress.subscribe([&](const MouseButtonEvent& e) {
        handleButtonPress(e);
    });

    _invalidate();
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

void Widget::updateArea(const Rectangle& parent) {
    if (m_isOutdated) {
        _updateMinimumSize();

        m_outerArea = m_properties.constraints.getRect(parent, m_properties, m_minSize);
        applyPadding();

        _updateArea();
    }
}

void Widget::updateScreenElements() {
    if (m_isOutdated) {
        m_renderQuadBackground.resize(m_outerArea);
        _updateScreenElements();
        m_isOutdated = false;
    }
}

UiProperties& Widget::properties() {
    return m_properties;
}

const glm::vec2& Widget::_getPosition() const {
    return m_innerArea.position;
}

const glm::vec2& Widget::_getSize() const {
    return m_innerArea.size;
}

const Rectangle& Widget::_getInnerArea() const {
    return m_outerArea;
}
const Rectangle& Widget::_getOuterArea() const {
    return m_innerArea;
}

void Widget::_setPosition(const glm::vec2& position) {
    m_innerArea.position = position;

    // readjust inner positions
    m_outerArea.position.x = m_innerArea.position.x + m_properties.padding.left;
    m_outerArea.position.y = m_innerArea.position.y + m_properties.padding.bottom;
    _invalidate();
}

void Widget::_setSize(const glm::vec2& size) {
    m_innerArea.size = size;

    // readjust inner sizes
    m_outerArea.size.x = m_innerArea.size.y - m_properties.padding.left - m_properties.padding.right;
    m_outerArea.size.y = m_innerArea.size.y - m_properties.padding.top - m_properties.padding.bottom;
    _invalidate();
}

bool Widget::isHovering() const {
    return m_isHovering;
};

bool Widget::isPressed() const {
    return m_isPressed;
}

const std::string& Widget::getId() const {
    return m_id;
}

void Widget::_setParent(Layout* parent) {
    m_parent = parent;
}

void Widget::_invalidate(bool propagate) {
    m_isOutdated = true;
    if (propagate && m_parent) {
        m_parent->_invalidate();
    }
}
