#include "../../include/Gui/MainMenu.h"

#include "../../include/Gui/Text.h"
#include "../../include/ResourceManager.h"

MainMenu::MainMenu()
    : GUI() {
    // add elements
    // text being abused as button
    Text* t = m_rootLayout->addWidget<Text>("button_quit");
    t->getProperties().backgroundColor = Color::Gray;
    t->getProperties().foregroundColor = Color::Emerald;
    t->getProperties().constraints.x = CenterConstraint();
    t->getProperties().constraints.y = RelativeConstraint(0.3);
    t->getProperties().padding.both(20);
    t->setFont(ResourceManager::getResource<Font>("fontKoruri"));
    t->setString("Quit");

    t = m_rootLayout->addWidget<Text>("button_start");
    t->getProperties().backgroundColor = Color::Gray;
    t->getProperties().foregroundColor = Color::Emerald;
    t->getProperties().constraints.x = CenterConstraint();
    t->getProperties().constraints.y = RelativeConstraint(0.6);
    t->getProperties().padding.both(20);
    t->setFont(ResourceManager::getResource<Font>("fontKoruri"));
    t->setString("Start");
}

MainMenu::~MainMenu() {

}
