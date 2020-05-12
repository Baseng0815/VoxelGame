#include "../include/Layout.h"

#include "../include/GUI.h"
#include "../include/Shader.h"

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

void Layout::resize() {
    if (isRootLayout()) {
        m_properties.position = glm::vec2(0, 0);
        m_properties.size = glm::vec2(1, 1);
    } else {
        auto& propertiesParent = m_parent->getProperties();
        m_area.position = propertiesParent.position + m_properties.position * propertiesParent.size;
        m_area.size = m_properties.size * propertiesParent.size;
        m_renderQuad.resize(m_area);
    }

    // a layout has to additionally resize all children
    for (auto widget : m_widgets)
        widget->resize();
}

template<class T>
T* Layout::addWidget(std::string id) {
    T* widget = new T(id, m_gui, this);
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
template Layout* Layout::addWidget<Layout>(std::string);
