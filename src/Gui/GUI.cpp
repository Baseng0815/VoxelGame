#include "../../include/Gui/GUI.h"
#include "../../include/Gui/Text.h"
#include "../../include/Gui/Image.h"

#include "../../include/Resources/Shader.h"

#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"
#include "../../include/Resources/ResourceManager.h"

#include <glm/gtc/matrix_transform.hpp>

// on framebuffer resize, all widgets are invalidated
void GUI::handleFramebufferSize(const FramebufferSizeEvent& e) {
    m_orthoProjection = glm::ortho(0.f, e.width, 0.f, e.height);
    for (auto &widget : m_widgets) {
        widget.second->_invalidate(false);
    }
}

GUI::GUI()
    : m_rootLayout {new Layout {"root_layout", *this}}
{
    m_rootLayout->properties().constraints.width = RelativeConstraint(1);
    m_rootLayout->properties().constraints.height = RelativeConstraint(1);

    m_framebufferSizeHandle = EventDispatcher::onFramebufferSize.subscribe([&](const FramebufferSizeEvent &e) {
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
    m_rootLayout->draw(m_orthoProjection);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void GUI::update() {
    m_rootLayout->updateArea(Rectangle { glm::vec2 {0.f, 0.f}, EventDispatcher::getFramebufferSize()});
    m_rootLayout->updateScreenElements();
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
    m_rootLayout->addWidget(panel);
}

void GUI::__registerWidget(Widget* widget) {
    m_widgets.insert(std::make_pair(widget->getId(), widget));
}
