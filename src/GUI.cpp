#include "../include/GUI.h"

#include "../include/Shader.h"
#include "../include/EventDispatcher.h"
#include "../include/ResourceManager.h"

void GUI::handleFramebufferSize(Event* event) {
}

void GUI::handleCursorMove(Event* e) {
}

void GUI::handleButtonPress(Event* e) {
}

void GUI::handleKeyPress(Event* e) {
}

GUI::GUI() {
    m_guiShader = ResourceManager::getResource<Shader>("guiShader");

    Layout* rootLayout = new Layout("root_layout", this);
    UiProperties* properties = rootLayout->getProperties();
    properties->backgroundColor = glm::vec4(0, 0, 0, 0);

    Layout* l1 = rootLayout->addWidget<Layout>("l1");
    properties = l1->getProperties();
    properties->position.y = 0.66;
    properties->size.y = 0.34;
    properties->backgroundColor = glm::vec4(0, 0, 0, 0.8);

    Layout* l2 = rootLayout->addWidget<Layout>("l2");
    properties = l2->getProperties();
    properties->position.y = 0.33;
    properties->size.y = 0.33;
    properties->backgroundColor = glm::vec4(1, 0, 0, 0.8);

    Layout* l3 = rootLayout->addWidget<Layout>("l3");
    properties = l3->getProperties();
    properties->size.y = 0.34;
    properties->backgroundColor = glm::vec4(1, 1, 0, 0.8);

    rootLayout->resize();

    ADD_EVENT(handleFramebufferSize, FRAMEBUFFER_SIZE_EVENT);
    ADD_EVENT(handleCursorMove, CURSOR_EVENT);
    ADD_EVENT(handleButtonPress, CURSOR_EVENT);
    ADD_EVENT(handleKeyPress, KEY_EVENT);
}

GUI::~GUI() {
    for (auto widget : m_widgets)
        delete widget.second;
}

void GUI::draw() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    m_guiShader->bind();

    // a layout is responsible to draw its children
    m_widgets["root_layout"]->draw(*m_guiShader);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void GUI::registerWidget(Widget* widget) {
    m_widgets.insert(std::make_pair(widget->getId(), widget));
}
