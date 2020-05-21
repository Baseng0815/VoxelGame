#include "../include/Layout.h"

#include "../include/GUI.h"
#include "../include/Shader.h"
#include "../include/Configuration.h"

#include "../include/Text.h"

#include <algorithm>

void Layout::arrangeWidgets() {
    // default layout does nothing special
}

void Layout::_draw(Shader& shader) const {
    for (auto widget : m_widgets)
        widget->draw(shader);
}

Layout::Layout(std::string id, GUI* gui, Layout* parent)
    : Widget(id, parent), m_gui(gui) {
    m_gui->registerWidget(this);
}

#include <iostream>

void Layout::resize(Rectangle parent) {
    if (isRootLayout()) {
        m_area = parent;
    } else {
        m_renderQuadBackground.resize(m_area);
    }

    // a layout has to additionally resize all children
    for (auto widget : m_widgets)
        widget->resize();
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
