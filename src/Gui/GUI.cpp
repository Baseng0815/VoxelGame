#include "../../include/Gui/GUI.h"
#include "../../include/Gui/Text.h"
#include "../../include/Gui/Image.h"

#include "../../include/Resources/Shader.h"

#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"
#include "../../include/ResourceManager.h"

#include <glm/gtc/matrix_transform.hpp>


void GUI::handleFramebufferSize(const FramebufferSizeEvent& e) {
    m_orthoProjection = glm::ortho(0.f, e.width, 0.f, e.height);

    m_rootLayouts.back()->updateArea(Rectangle(0, 0, e.width, e.height));
    m_rootLayouts.back()->updateScreenElements();
}

GUI::GUI() {
    Layout *m_rootLayout = new Layout("root_layout", *this);
    m_rootLayout->properties().constraints.width = RelativeConstraint(1);
    m_rootLayout->properties().constraints.height = RelativeConstraint(1);

    m_rootLayouts.push_back(m_rootLayout);

    m_framebufferSizeHandle = EventDispatcher::onFramebufferSize.subscribe([this](const FramebufferSizeEvent& e) {
        handleFramebufferSize(e);
    });
}

GUI::~GUI() {
    for (auto widget : m_widgets)
        delete widget.second;
}

void GUI::draw() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // a layout is responsible to draw its children
    for (auto &rootLayout : m_rootLayouts) {
        rootLayout->draw(m_orthoProjection);
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void GUI::update() {
    for (auto &rootLayout : m_rootLayouts) {
        rootLayout->updateArea(rootLayout->_getOuterArea());
        rootLayout->updateScreenElements();
    }
}

Widget& GUI::getWidget(const std::string& id) {
    auto it = m_widgets.find(id);
    if (it != m_widgets.end()) {
        return *(it->second);
    } else {
        std::cout << "Widget with ID " << id << " not found" << std::endl;
    }
}

void GUI::addPanel(Layout* panel) {
    static_cast<Layout*>(m_widgets["root_layout"])->addWidget(panel);
}

void GUI::__registerWidget(Widget* widget) {
    m_widgets.insert(std::make_pair(widget->getId(), widget));
}
