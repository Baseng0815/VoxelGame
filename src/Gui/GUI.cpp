#include "../../include/Gui/GUI.h"
#include "../../include/Gui/Text.h"

#include "../../include/Rendering/Shader.h"

#include "../../include/Configuration.h"
#include "../../include/EventDispatcher.h"
#include "../../include/ResourceManager.h"
#include "../../include/Components/CameraComponent.h"

#include <glm/gtc/matrix_transform.hpp>

void GUI::handleFramebufferSize(Event* event) {
    FramebufferSizeEvent e = *event->get<FramebufferSizeEvent>();

    m_widgets["root_layout"]->updateArea(Rectangle(0, 0, e.width, e.height));
    m_widgets["root_layout"]->updateScreenElements();
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
    rootLayout->getProperties().constraints.width = RelativeConstraint(1);
    rootLayout->getProperties().constraints.height = RelativeConstraint(1);

    Layout* debugInfo = rootLayout->addWidget<Layout>("sublayout1");
    debugInfo->getProperties().constraints.width = RelativeConstraint(0.3);
    debugInfo->getProperties().constraints.height = RelativeConstraint(1);
    debugInfo->getProperties().backgroundColor = glm::vec4(.1, .1, .1, 0.5);
    debugInfo->setStackMode(STACK_HORIZONTAL, true, true);

    // TODO add margin/padding

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

    rootLayout->updateArea(Rectangle(0, 0,
                Configuration::INITIAL_WINDOW_WIDTH, Configuration::INITIAL_WINDOW_HEIGHT));
    rootLayout->updateScreenElements();

    EventDispatcher::addCallback(CB_FUN(handleFramebufferSize), FRAMEBUFFER_SIZE_EVENT);
    EventDispatcher::addCallback(CB_FUN(handleCursorMove), CURSOR_EVENT);
    EventDispatcher::addCallback(CB_FUN(handleButtonPress), CURSOR_EVENT);
    EventDispatcher::addCallback(CB_FUN(handleKeyPress), KEY_EVENT);
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
