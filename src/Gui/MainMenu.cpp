#include "../../include/Gui/MainMenu.h"

#include "../../include/Gui/Text.h"
#include "../../include/ResourceManager.h"

MainMenu::MainMenu()
    : GUI() {
    // add elements
    Text* t = m_rootLayout->addWidget<Text>("start");
    t->getProperties().constraints.x = CenterConstraint();
    t->getProperties().constraints.y = CenterConstraint();
    t->getProperties().backgroundColor = glm::vec4(1, 0, 0, 1);
    t->setFont(ResourceManager::getResource<Font>("fontKoruri"));
    t->setString("Hallo Welt");
}

MainMenu::~MainMenu() {

}
