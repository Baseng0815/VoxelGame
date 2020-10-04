#include "../../include/Gui/Text.hpp"
#include "../../include/Gui/Image.hpp"
#include "../../include/Gui/GUI.hpp"
//#include "../../include/Gui/Layout.hpp"
#include "../../include/Gui/ItemLayout.hpp"

#include "../../include/Resources/Shader.hpp"
#include "../../include/Configuration.hpp"

void Layout::arrangeWidgets() {
    if(m_widgets.size() == 0)
        return;
    
    // set position of widgets based on stack mode and widget dimensions
    int start = m_invertStackWidgets ? m_widgets.size() - 1 : 0;
    // end = first nonvalid element
    int end =   m_invertStackWidgets ? -1 : m_widgets.size();
    int direction = m_invertStackWidgets ? -1 : 1;

    if (m_stackMode == STACK_VERTICAL) {
        if (m_invertStack) { // UP
            int currentY = m_innerArea.position.y + m_widgets[0]->properties().margin.bottom;
            for (int i = start; i != end; i+= direction) {
                Widget& widget = *m_widgets[i];
                widget._setPosition(glm::vec2(widget._getPosition().x, currentY));
                if (i != end - direction) {
                    // margin and size from current element
                    currentY += widget._getSize().y + widget.properties().margin.top;
                    // margin from next element
                    currentY += m_widgets[i + direction]->properties().margin.bottom;
                }
            }
        } else { // DOWN
            int currentY = m_innerArea.position.y + m_innerArea.size.y - m_widgets[start]->_getSize().y;
            for (int i = start; i != end; i += direction) {
                Widget& widget = *m_widgets[i];
                widget._setPosition(glm::vec2(widget._getPosition().x, currentY));
                if (i != end - direction) {
                    // margin from current element
                    currentY -= widget.properties().margin.bottom;
                    // margin and size from next element
                    currentY -= m_widgets[i + direction]->properties().margin.top
                        + m_widgets[i + direction]->_getSize().y;
                }
            }
        }
    } else if (m_stackMode == STACK_HORIZONTAL) {
        if (m_invertStack) { // LEFT
            int currentX = m_innerArea.position.x + m_innerArea.size.x - m_widgets[start]->_getSize().x;
            for (int i = start; i != end; i += direction) {
                Widget &widget = *m_widgets[i];
                widget._setPosition(glm::vec2(currentX, widget._getPosition().y));
                if (i != end - direction) {
                    // margin from current element
                    currentX -= widget.properties().margin.left;
                    // margin and size from next element
                    currentX -= m_widgets[i + direction]->properties().margin.right + m_widgets[i + direction]->_getSize().x;
                }
            }            
        } else { // RIGHT
            int currentX = m_innerArea.position.x;
            for (int i = start; i != end; i += direction) {
                Widget &widget = *m_widgets[i];
                widget._setPosition(glm::vec2(currentX, widget._getPosition().y));
                if (i != end - direction) {
                    // margin and size from current element
                    currentX += widget._getSize().x + widget.properties().margin.right;
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

void Layout::_updateArea()
{
    // update all child areas
    for (auto widget : m_widgets)
        widget->updateArea(m_innerArea);

    // arrange all children
    arrangeWidgets();
}

void Layout::_updateScreenElements()
{
    for (auto widget : m_widgets)
        widget->updateScreenElements();

    m_renderQuadBackground.resize(m_outerArea);
}

Layout::Layout(const std::string &id, GUI &gui)
    : Widget {id}, m_gui {gui}
{ }

void Layout::setStackMode(StackMode stackMode, bool invertStack, bool invertStackWidgets)
{
    m_stackMode = stackMode;
    m_invertStack = invertStack;
    m_invertStackWidgets = invertStackWidgets;
}

template<typename T>
T *Layout::addWidget(const std::string& id)
{
    T* widget = new T {id};
    widget->_setParent(this);
    m_widgets.push_back(widget);
    m_gui.__registerWidget(widget);
    return widget;
}

template<>
Layout *Layout::addWidget(const std::string& id)
{
    Layout* widget = new Layout {id, m_gui};
    widget->_setParent(this);
    m_widgets.push_back(widget);
    m_gui.__registerWidget(widget);
    return widget;
}

Widget *Layout::addWidget(Widget* widget)
{
    m_widgets.push_back(widget);
    widget->_setParent(this);
    m_gui.__registerWidget(widget);
    return widget;
}

void Layout::removeWidget(const std::string& id) {
    int i = 0;
    while(i < m_widgets.size()) {
        if (m_widgets[i]->getId() == id) {
            break;
        }
        else {
            i++;
        }
    }

    Widget *widget = m_widgets[i];
    m_gui.__unregisterWidget(widget);
    m_widgets.erase(m_widgets.begin() + i);

    delete widget;
}

void Layout::clearWidgets() {
    while (m_widgets.size() > 0) {
        Widget *widget = m_widgets.front();

        m_gui.__unregisterWidget(widget);
        m_widgets.erase(m_widgets.begin());

        delete widget;
    }
}

void Layout::_setPosition(const glm::vec2 & position) {
    Widget::_setPosition(position);

    for (auto widget : m_widgets) {
        widget->updateArea(m_innerArea);
    }

    arrangeWidgets();
}

WidgetIt Layout::begin()
{ return m_widgets.begin(); }

WidgetIt Layout::end()
{ return m_widgets.end(); }

// predefine templates here
template Text* Layout::addWidget<Text>(const std::string&);
template Image* Layout::addWidget<Image>(const std::string&);
