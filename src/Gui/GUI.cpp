#include "../../include/Gui/GUI.h"
#include "../../include/Gui/Text.h"

#include "../../include/Rendering/Shader.h"

#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"
#include "../../include/ResourceManager.h"

#include <glm/gtc/matrix_transform.hpp>

bool GUI::coordinatesInWidget(const Widget& widget, int x, int y) {
    Rectangle widgetArea = widget.getArea();
    if (x > widgetArea.position.x && x < widgetArea.position.x + widgetArea.size.x &&
        y > widgetArea.position.y && y < widgetArea.position.y + widgetArea.size.y)
        return true;
    else return false;
}

void GUI::handleFramebufferSize(const FramebufferSizeEvent& e) {
    m_orthoProjection = glm::ortho(0.f, e.width, 0.f, e.height);

    m_widgets["root_layout"]->updateArea(Rectangle(0, 0, e.width, e.height));
    m_widgets["root_layout"]->updateScreenElements();
}

void GUI::handleCursorMove(const CursorEvent& e) {
    for (auto widget = m_rootLayout->begin(); widget != m_rootLayout->end(); widget++) {
        // cursor y position is inverted in GUI space
        bool inArea = coordinatesInWidget(*(*widget), e.x, EventDispatcher::getFramebufferSize().y - e.y);
        if ((*widget)->m_isHovering) {
            if (!inArea) {
                (*widget)->onLeave.invoke(e.x, e.y);
                (*widget)->m_isHovering = false;
            }
            else {
                (*widget)->onMove.invoke(e.x, e.y);
            }
        } else {
            if (inArea) {
                (*widget)->onEnter.invoke(e.x, e.y);
                (*widget)->m_isHovering = true;
            }
        }
    }
}

void GUI::handleButtonPress(const MouseButtonEvent& e) {
    for (auto widget = m_rootLayout->begin(); widget != m_rootLayout->end(); widget++) {
        if (!(*widget)->m_isHovering) continue;
        if (e.action == GLFW_PRESS) {
            (*widget)->onPress.invoke(0, 0);
            (*widget)->m_isPressed = true;
        }
        else if (e.action == GLFW_RELEASE) {
            (*widget)->onRelease.invoke(0, 0);
            (*widget)->m_isPressed = false;
        }
    }
}

void GUI::handleKeyPress(const KeyEvent& e) {
}

GUI::GUI() {
    m_rootLayout = new Layout("root_layout", this);
    m_rootLayout->getProperties().constraints.width = RelativeConstraint(1);
    m_rootLayout->getProperties().constraints.height = RelativeConstraint(1);

    // TODO move into debug layout
    /*
    Layout* debugInfo = m_rootLayout->addWidget<Layout>("sublayout1");
    debugInfo->getProperties().constraints.width = RelativeConstraint(0.3);
    debugInfo->getProperties().constraints.height = RelativeConstraint(1);
    debugInfo->getProperties().backgroundColor = glm::vec4(.1, .1, .1, 0.5);
    debugInfo->setStackMode(STACK_HORIZONTAL, true, true);

    Text* t = debugInfo->addWidget<Text>("text_fps");
    t->getProperties().constraints.x = CenterConstraint();
    t->getProperties().backgroundColor = glm::vec4(1, 0, 0, 0.5);
    t->getProperties().margin.bottom = 100;
    t->setFont(ResourceManager::getResource<Font>("fontKoruri"));
    t->setString("1) FPS Counter");

    t = debugInfo->addWidget<Text>("text_fps");
    t->getProperties().constraints.x = CenterConstraint();
    t->getProperties().backgroundColor = glm::vec4(0, 0, 1, 0.5);
    t->setFont(ResourceManager::getResource<Font>("fontKoruri"));
    t->setString("2) Number of chunks");

    t = debugInfo->addWidget<Text>("text_fps");
    t->getProperties().constraints.x = CenterConstraint();
    t->getProperties().backgroundColor = glm::vec4(1, 1, 0, 0.5);
    t->setFont(ResourceManager::getResource<Font>("fontKoruri"));
    t->setString("3) Memory count");

    t = debugInfo->addWidget<Text>("text_fps");
    t->getProperties().constraints.x = CenterConstraint();
    t->getProperties().backgroundColor = glm::vec4(0, 1, 0, 0.5);
    t->setFont(ResourceManager::getResource<Font>("fontKoruri"));
    t->setString("4) Number of allocations/more memory stats");

    t = debugInfo->addWidget<Text>("text_fps");
    t->getProperties().constraints.x = CenterConstraint();
    t->setFont(ResourceManager::getResource<Font>("fontKoruri"));
    t->setString("5) Camera position and direction");
    */

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
    m_rootLayout->draw(m_orthoProjection);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void GUI::invalidate() {
    m_isOutdated = true;
}

void GUI::update() {
    if (m_isOutdated) {
        m_rootLayout->updateArea(m_rootLayout->getArea());
        m_rootLayout->updateScreenElements();

        m_isOutdated = false;
    }
}

void GUI::registerWidget(Widget* widget) {
    m_widgets.insert(std::make_pair(widget->getId(), widget));
}
