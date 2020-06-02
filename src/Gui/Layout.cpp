#include "../../include/Gui/Text.h"
#include "../../include/Gui/GUI.h"
#include "../../include/Gui/Layout.h"

#include "../../include/Rendering/Shader.h"
#include "../../include/Configuration.h"

#include <algorithm>

void Layout::arrangeWidgets() {
    // set position of widgets based on stack mode and widget dimensions
    int start = m_invertStackWidgets ? m_widgets.size() - 1 : 0;
    // end = first nonvalid element
    int end =   m_invertStackWidgets ? -1 : m_widgets.size();
    int direction = m_invertStackWidgets ? -1 : 1;

    if (m_stackMode == STACK_VERTICAL) {
        if (m_invertStack) { // DOWN
            int currentY = m_finalArea.position.y + m_finalArea.size.y - m_widgets[start]->getSize().y;
            for (int i = start; i != end; i += direction) {
                Widget& widget = *m_widgets[i];
                widget.setPosition(glm::vec2(widget.getPosition().x, currentY));
                if (i != end - direction) {
                    // margin from current element
                    currentY -= widget.getProperties().margin.bottom;
                    // margin and size from next element
                    currentY -= m_widgets[i + direction]->getProperties().margin.top
                        + m_widgets[i + direction]->getSize().y;
                }
            }
        } else { // UP
            int currentY = m_finalArea.position.y;
            for (int i = start; i != end; i+= direction) {
                Widget& widget = *m_widgets[i];
                widget.setPosition(glm::vec2(widget.getPosition().x, currentY));
                if (i != end - direction) {
                    // margin and size from current element
                    currentY += widget.getSize().y + widget.getProperties().margin.top;
                    // margin from next element
                    currentY += m_widgets[i + direction]->getProperties().margin.bottom;
                }
            }
        }
    } else if (m_stackMode == STACK_HORIZONTAL) {
        if (m_invertStack) { // LEFT
            int currentX = m_finalArea.position.x + m_finalArea.size.x - m_widgets[start]->getSize().x;
            for (int i = start; i != end; i += direction) {
                Widget& widget = *m_widgets[i];
                widget.setPosition(glm::vec2(currentX, widget.getPosition().y));
                if (i != end - direction) {
                    // margin from current element
                    currentX -= widget.getProperties().margin.left;
                    // margin and size from next element
                    currentX -= m_widgets[i + direction]->getProperties().margin.right
                        + m_widgets[i + direction]->getSize().x;
                }
            }
        } else { // RIGHT
            int currentX = m_finalArea.position.x;
            for (int i = start; i != end; i+= direction) {
                Widget& widget = *m_widgets[i];
                widget.setPosition(glm::vec2(currentX, widget.getPosition().y));
                if (i != end - direction) {
                    // margin and size from current element
                    currentX += widget.getSize().x + widget.getProperties().margin.right;
                    // margin from next element
                    currentX += m_widgets[i + direction]->getProperties().margin.left;
                }
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

void Layout::setStackMode(StackMode stackMode, bool invertStack, bool invertStackWidgets) {
    m_stackMode = stackMode;
    m_invertStack = invertStack;
    m_invertStackWidgets = invertStackWidgets;
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
