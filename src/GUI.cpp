#include "../include/GUI.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../include/Shader.h"
#include "../include/Configuration.h"
#include "../include/EventDispatcher.h"
#include "../include/ResourceManager.h"
#include "../include/Components/CameraComponent.h"

#include "../include/Text.h"

void GUI::handleFramebufferSize(Event* event) {
    FramebufferSizeEvent e = *event->get<FramebufferSizeEvent>();

    m_widgets["root_layout"]->resize(Rectangle(0, 0, e.width, e.height));
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

    /*
    Text* text = rootLayout->addWidget<Text>("text_1");
    text->getProperties().constraints.x = PixelConstraint(0);
    text->getProperties().constraints.y = PixelConstraint(0);
    text->getProperties().constraints.width = MatchConstraint();
    text->getProperties().constraints.height = MatchConstraint();
    text->setFont(ResourceManager::getResource<Font>("fontKoruri"));
    text->setString("Hallo Welt");
    */

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

void GUI::draw(const CameraComponent& camera) {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    m_guiShader->bind();
    m_guiShader->upload("projectionMatrix", camera.orthoProjection);

    // a layout is responsible to draw its children
    m_widgets["root_layout"]->draw(*m_guiShader);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void GUI::registerWidget(Widget* widget) {
    m_widgets.insert(std::make_pair(widget->getId(), widget));
}
