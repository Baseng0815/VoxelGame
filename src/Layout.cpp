#include "../include/Layout.h"

#include "../include/GUI.h"
#include "../include/Shader.h"
#include "../include/Configuration.h"

#include "../include/Text.h"

#include <algorithm>

// TODO remove
#include <iostream>

void Layout::arrangeWidgets() {
    // set position of widgets based on stack mode and widget dimensions
    if (m_stackMode == STACK_VERTICAL) {
        if (m_invertStack) {
            int currentY = -30 + m_finalArea.position.y + m_finalArea.size.y - m_widgets.back()->getSize().y;
            for (auto it = m_widgets.rbegin(); it != m_widgets.rend(); it++) {
                (*it)->setPosition(glm::vec2((*it)->getPosition().x, currentY));
                // TODO use margin/padding
                currentY -= 2 * (*it)->getSize().y;
            }
        } else {
            int currentY = m_finalArea.position.y;
            for (auto widget : m_widgets) {
                widget->setPosition(glm::vec2(widget->getPosition().x, currentY));
                currentY += widget->getSize().y;
            }
        }
    } else if (m_stackMode == STACK_HORIZONTAL) {
        if (m_invertStack) {
            int currentX = m_finalArea.position.x + m_finalArea.size.x - m_widgets.back()->getSize().x;
            for (auto it = m_widgets.rbegin(); it != m_widgets.rend(); it++) {
                (*it)->setPosition(glm::vec2(currentX, (*it)->getPosition().y));
                currentX -= (*it)->getSize().x;
            }
        } else {
            int currentX = m_finalArea.position.x;
            for (auto widget : m_widgets) {
                // TODO maybe change m_finalArea.position.y to widget.getPosition().y ?
                widget->setPosition(glm::vec2(currentX, m_finalArea.position.y));
                currentX += widget->getSize().x;
            }
        }
    }
}

void Layout::_draw(Shader& shader) const {
    for (auto widget : m_widgets)
        widget->draw(shader);
}

Layout::Layout(std::string id, GUI* gui)
    : Widget(id), m_gui(gui) {
        m_gui->registerWidget(this);
    }

void Layout::setStackMode(StackMode stackMode, bool invertStack) {
    m_stackMode = stackMode;
    m_invertStack = invertStack;
}

void Layout::updateArea(const Rectangle& parent) {
    m_finalArea = m_properties.constraints.getRect(parent, m_minWidth, m_minHeight);

    // update all child areas
    for (auto widget : m_widgets)
        widget->updateArea(m_finalArea);
}

void Layout::updateScreenElements() {
    arrangeWidgets();

    for (auto widget : m_widgets)
        widget->updateScreenElements();

    m_renderQuadBackground.resize(m_finalArea);
}

template<typename T>
T* Layout::addWidget(std::string id) {
    T* widget = new T(id);
    m_widgets.push_back(widget);
    m_gui->registerWidget(widget);
    return widget;
}

template<>
Layout* Layout::addWidget(std::string id) {
    Layout* widget = new Layout(id, m_gui);
    m_widgets.push_back(widget);
    m_gui->registerWidget(widget);
    return widget;
}

Widget* Layout::addWidget(Widget* widget) {
    m_widgets.push_back(widget);
    return widget;
}

WidgetIt Layout::begin() {
    return m_widgets.begin();
}

WidgetIt Layout::end() {
    return m_widgets.end();
}

// predefine templates here
template Text* Layout::addWidget<Text>(std::string);
