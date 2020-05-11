#include "../include/Layout.h"

#include "../include/GUI.h"

void Layout::arrangeWidgets() {
    // default layout does nothing special
}

Layout::Layout(std::string id, GUI* gui, Layout* parent)
    : Widget(id, parent), m_gui(gui) {
    m_gui->registerWidget(this);
}

void Layout::draw() const {
    for (auto widget : m_widgets)
        widget->draw();
}

void Layout::resize(const Rectangle& parent) {
    m_area.position = parent.position + m_properties.position * parent.size;
    m_area.size = m_properties.size * parent.size;

    // a layout has to additionally resize all children
    for (auto widget : m_widgets)
        widget->resize(m_area);
}

template<class T>
T* Layout::addWidget(std::string id) {
    T* widget = new T(id, m_gui, this);
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
