#include "../../include/Gui/MainMenuLayout.h"

#include "../../include/Gui/Text.h"
#include "../../include/Gui/Image.h"
#include "../../include/ResourceManager.h"

MainMenuLayout::MainMenuLayout(GUI* gui)
    : Layout("layout_mainmenu", gui) {
    m_properties.constraints.width = RelativeConstraint(1);
    m_properties.constraints.height = RelativeConstraint(1);

    // add elements
    Image* img = new Image("image_background", gui, ResourceManager::getResource<Texture>("textureBackgroundMainMenu"));
    img->getProperties().constraints.width = RelativeConstraint(1);
    img->getProperties().constraints.height = RelativeConstraint(1);
    this->addWidget(img);

    // text being (ab)used as button
    Text* t = this->addWidget<Text>("button_quit");
    t->getProperties().backgroundColor = Color::Gray;
    t->getProperties().foregroundColor = Color::Emerald;
    t->getProperties().constraints.x = CenterConstraint();
    t->getProperties().constraints.y = RelativeConstraint(0.3);
    t->getProperties().padding.both(20);
    t->setFont(ResourceManager::getResource<Font>("fontKoruri"));
    t->setString("Quit");

    t = this->addWidget<Text>("button_start");
    t->getProperties().backgroundColor = Color::Gray;
    t->getProperties().foregroundColor = Color::Emerald;
    t->getProperties().constraints.x = CenterConstraint();
    t->getProperties().constraints.y = RelativeConstraint(0.6);
    t->getProperties().padding.both(20);
    t->setFont(ResourceManager::getResource<Font>("fontKoruri"));
    t->setString("Start");
}
