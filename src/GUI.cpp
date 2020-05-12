#include "../include/GUI.h"

#include "../include/Shader.h"
#include "../include/EventDispatcher.h"
#include "../include/ResourceManager.h"

// TODO remove
#include <iostream>

void GUI::handleFramebufferSize(Event* event) {
}

void GUI::handleCursorMove(Event* e) {
}

void GUI::handleButtonPress(Event* e) {
}

GUI::GUI() {
    m_guiShader = ResourceManager::getResource<Shader>("guiShader");

    Layout* rootLayout = new Layout("root_layout", this);
    auto& propertiesRoot = rootLayout->getProperties();
    propertiesRoot.backgroundColor = glm::vec3(0, 0, 1);

    Layout* sublayout = rootLayout->addWidget<Layout>("sub_layout");
    auto& propertiesSub = sublayout->getProperties();
    propertiesSub.backgroundColor = glm::vec3(1, 0, 0);
    propertiesSub.size = glm::vec2(.5, .5);

    rootLayout->resize();

    ADD_EVENT(handleFramebufferSize, FRAMEBUFFER_SIZE_EVENT);
    ADD_EVENT(handleCursorMove, CURSOR_EVENT);
    ADD_EVENT(handleButtonPress, CURSOR_EVENT)
}

GUI::~GUI() {
    for (auto widget : m_widgets)
        delete widget.second;
}

void GUI::draw() {
    m_guiShader->bind();

    // a layout is responsible to draw its children
    m_widgets["root_layout"]->draw(*m_guiShader);
}

void GUI::registerWidget(Widget* widget) {
    m_widgets.insert(std::make_pair(widget->getId(), widget));
}
