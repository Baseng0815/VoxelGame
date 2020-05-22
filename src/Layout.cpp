#include "../include/Layout.h"

#include "../include/GUI.h"
#include "../include/Shader.h"
#include "../include/Configuration.h"

#include "../include/Text.h"

#include <algorithm>

void Layout::arrangeWidgets() {
    // set position of widgets based on stack mode and widget dimensions
    switch (m_stackMode) {
        case STACK_HORIZONTAL: {
            int xoff = 0;
            for (auto widget : m_widgets) {
                widget->setPosition(widget->getPosition() + glm::vec2(xoff, 0));
                if (!m_invertStack)
                    xoff += widget->getSize().x;
                else
                    xoff -= widget->getSize().x;
            }
            break;
        }

        case STACK_VERTICAL: {
            int yoff = 0;
            for (auto widget : m_widgets) {
                widget->setPosition(widget->getPosition() + glm::vec2(0, yoff));
                if (!m_invertStack)
                    yoff -= widget->getSize().y;
                else
                    yoff += widget->getSize().y;
            }
            break;
        }

        default:
            break;
    }
}

void Layout::_draw(Shader& shader) const {
    for (auto widget : m_widgets)
        widget->draw(shader);
}

Layout::Layout(std::string id, GUI* gui, Layout* parent)
    : Widget(id, parent), m_gui(gui) {
    m_gui->registerWidget(this);
}

void Layout::setStackMode(StackMode stackMode, bool invertStack) {
    m_stackMode = stackMode;
    m_invertStack = invertStack;
}

void Layout::updateArea(Rectangle parent) {
    if (isRootLayout()) {
        m_area = parent;
    } else {
        m_area = m_properties.constraints.getRect(parent);
    }

    // update all child areas
    for (auto widget : m_widgets)
        widget->updateArea();
}

void Layout::updateScreenElements() {
    arrangeWidgets();

    for (auto widget : m_widgets)
        widget->updateScreenElements();

    m_renderQuadBackground.resize(m_area);
}

template<typename T>
T* Layout::addWidget(std::string id) {
    T* widget = new T(id, this);
    m_widgets.push_back(widget);
    m_gui->registerWidget(widget);
    return widget;
}

template<>
Layout* Layout::addWidget(std::string id) {
    Layout* widget = new Layout(id, m_gui, this);
    m_widgets.push_back(widget);
    m_gui->registerWidget(widget);
    return widget;
}

Widget* Layout::addWidget(Widget* widget) {
    m_widgets.push_back(widget);
    return widget;
}

bool Layout::isRootLayout() const {
    return m_parent == nullptr;
}

WidgetIt Layout::begin() {
    return m_widgets.begin();
}

WidgetIt Layout::end() {
    return m_widgets.end();
}

// predefine templates here
template Text* Layout::addWidget<Text>(std::string);
