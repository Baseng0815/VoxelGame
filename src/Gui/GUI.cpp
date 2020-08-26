#include "../../include/Gui/GUI.h"
#include "../../include/Gui/Text.h"
#include "../../include/Gui/Image.h"

#include "../../include/Resources/Shader.h"

#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"
#include "../../include/ResourceManager.h"

#include <glm/gtc/matrix_transform.hpp>

bool GUI::coordinatesInWidget(const Widget &widget, int x, int y) {
    const Rectangle &innerArea = widget.getInnerArea();
    if (x > innerArea.position.x && x < innerArea.position.x + innerArea.size.x &&
            y > innerArea.position.y && y < innerArea.position.y + innerArea.size.y)
        return true;
    else return false;
}

void GUI::handleFramebufferSize(const FramebufferSizeEvent& e) {
    m_orthoProjection = glm::ortho(0.f, e.width, 0.f, e.height);

    m_rootLayouts.top()->updateArea(Rectangle(0, 0, e.width, e.height));
    m_rootLayouts.top()->updateScreenElements();
}

void GUI::handleCursorMove(const CursorEvent &e) {
    for (auto widget = m_widgets.begin(); widget != m_widgets.end(); widget++) {
        if (!widget->second->properties().receiveEvents) continue;

        // cursor y position is inverted in GUI space
        bool inArea = coordinatesInWidget(*(widget->second), e.x, EventDispatcher::getFramebufferSize().y - e.y);
        if (widget->second->isHovering()) {
            if (!inArea) {
                widget->second->onLeave.invoke(e);
                widget->second->m_isHovering = false;
            }
            else {
                widget->second->onMove.invoke(e);
            }
        } else {
            if (inArea) {
                widget->second->onEnter.invoke(e);
                widget->second->m_isHovering = true;
            }
        }
    }
}

void GUI::handleButtonPress(const MouseButtonEvent& e) {
    for (auto widget = m_widgets.begin(); widget != m_widgets.end(); widget++) {
        if (!widget->second->getProperties().receiveEvents) continue;
        if (!widget->second->m_isHovering) continue;

        if (e.action == GLFW_PRESS) {
            widget->second->onPress.invoke(e);
            widget->second->m_isPressed = true;
        }
        else if (e.action == GLFW_RELEASE) {
            widget->second->onRelease.invoke(e);
            widget->second->m_isPressed = false;
        }
    }
}

void GUI::handleKeyPress(const KeyEvent& e) {
}

GUI::GUI() {
    Layout *m_rootLayout = new Layout("root_layout", this);
    m_rootLayout->getProperties().constraints.width = RelativeConstraint(1);
    m_rootLayout->getProperties().constraints.height = RelativeConstraint(1);

    m_rootLayouts.push_back(m_rootLayout);

    EventDispatcher::onFramebufferSize += [this](const FramebufferSizeEvent& e) {
        handleFramebufferSize(e);
    };
    EventDispatcher::onCursorMove += [this](const CursorEvent& e) {
        handleCursorMove(e);
    };
    EventDispatcher::onMouseButtonPress += [this](const MouseButtonEvent& e) {
        handleButtonPress(e);
    };
    EventDispatcher::onKeyPress += [this](const KeyEvent& e) {
        handleKeyPress(e);
    };
}

GUI::~GUI() {
    for (auto widget : m_widgets)
        delete widget.second;
}

void GUI::draw() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // a layout is responsible to draw its children
    for (auto rootLayout : m_rootLayouts) {
        rootLayout->draw(m_orthoProjection);
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void GUI::__invalidate() {
    m_isOutdated = true;
}

void GUI::update() {
    if (m_isOutdated) {
        for (auto rootLayout : m_rootLayouts) {
            rootLayout->updateArea(rootLayout->getArea());
            rootLayout->updateScreenElements();
        }

        m_isOutdated = false;
    }
}

void GUI::__registerWidget(Widget* widget) {
    m_widgets.insert(std::make_pair(widget->getId(), widget));
}

Widget& GUI::getWidget(const std::string& id) {
    auto it = m_widgets.find(id);
    if (it != m_widgets.end())
        return *(it->second);
    else
        std::cout << "Widget with ID " << id << " not found" << std::endl;
}

void GUI::addPanel(Layout* panel) {
    static_cast<Layout*>(m_widgets["root_layout"])->addWidget(panel);
}
