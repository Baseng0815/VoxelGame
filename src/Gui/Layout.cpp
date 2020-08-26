#include "../../include/Gui/Text.h"
#include "../../include/Gui/Image.h"
#include "../../include/Gui/GUI.h"
#include "../../include/Gui/Layout.h"

#include "../../include/Resources//Shader.h"
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
            int currentY = m_outerArea.position.y + m_outerArea.size.y - m_widgets[start]->getSize().y;
            for (int i = start; i != end; i += direction) {
                Widget& widget = *m_widgets[i];
                widget.setPosition(glm::vec2(widget.getPosition().x, currentY));
                if (i != end - direction) {
                    // margin from current element
                    currentY -= widget.properties().margin.bottom;
                    // margin and size from next element
                    currentY -= m_widgets[i + direction]->properties().margin.top
                        + m_widgets[i + direction]->getSize().y;
                }
            }
        } else { // UP
            int currentY = m_outerArea.position.y + m_widgets[0]->properties().margin.bottom;
            for (int i = start; i != end; i+= direction) {
                Widget& widget = *m_widgets[i];
                widget.setPosition(glm::vec2(widget.getPosition().x, currentY));
                if (i != end - direction) {
                    // margin and size from current element
                    currentY += widget.getSize().y + widget.properties().margin.top;
                    // margin from next element
                    currentY += m_widgets[i + direction]->properties().margin.bottom;
                }
            }
        }
    } else if (m_stackMode == STACK_HORIZONTAL) {
        if (m_invertStack) { // LEFT
            int currentX = m_outerArea.position.x + m_outerArea.size.x - m_widgets[start]->getSize().x;
            for (int i = start; i != end; i += direction) {
                Widget& widget = *m_widgets[i];
                widget.setPosition(glm::vec2(currentX, widget.getPosition().y));
                if (i != end - direction) {
                    // margin from current element
                    currentX -= widget.properties().margin.left;
                    // margin and size from next element
                    currentX -= m_widgets[i + direction]->properties().margin.right
                        + m_widgets[i + direction]->getSize().x;
                }
            }
        } else { // RIGHT
            int currentX = m_outerArea.position.x;
            for (int i = start; i != end; i+= direction) {
                Widget& widget = *m_widgets[i];
                widget.setPosition(glm::vec2(currentX, widget.getPosition().y));
                if (i != end - direction) {
                    // margin and size from current element
                    currentX += widget.getSize().x + widget.properties().margin.right;
                    // margin from next element
                    currentX += m_widgets[i + direction]->properties().margin.left;
                }
            }
        }
    }
}

void Layout::_draw(const glm::mat4 &projection) const
{
    for (auto widget : m_widgets) {
        widget->draw(projection);
    }
}

void Layout::_updateArea() {
    // update all child areas
    for (auto widget : m_widgets)
        widget->updateArea(m_outerArea);

    // arrange all children
    arrangeWidgets();
}

Layout::Layout(const std::string &id, GUI *gui)
    : Widget(id), m_gui(gui) {
    m_gui->__registerWidget(this);
}

void Layout::setStackMode(StackMode stackMode, bool invertStack, bool invertStackWidgets) {
    m_stackMode = stackMode;
    m_invertStack = invertStack;
    m_invertStackWidgets = invertStackWidgets;
}


void Layout::updateScreenElements() {
    arrangeWidgets();

    for (auto widget : m_widgets)
        widget->updateScreenElements();

    m_renderQuadBackground.resize(m_outerArea);
}

template<typename T>
T* Layout::addWidget(const std::string& id) {
    T* widget = new T(id, m_gui);
    m_widgets.push_back(widget);
    m_gui->__registerWidget(widget);
    return widget;
}

template<>
Layout* Layout::addWidget(const std::string& id) {
    Layout* widget = new Layout(id, m_gui);
    m_widgets.push_back(widget);
    m_gui->__registerWidget(widget);
    return widget;
}

Widget* Layout::addWidget(Widget* widget) {
    m_widgets.push_back(widget);
    m_gui->__registerWidget(widget);
    return widget;
}

WidgetIt Layout::begin() {
    return m_widgets.begin();
}

WidgetIt Layout::end() {
    return m_widgets.end();
}

// predefine templates here
template Text* Layout::addWidget<Text>(const std::string&);
template Image* Layout::addWidget<Image>(const std::string&);
