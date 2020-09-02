#include "../../include/Gui/MainMenuLayout.h"

#include "../../include/Gui/Text.h"
#include "../../include/Gui/Image.h"
#include "../../include/Resources/ResourceManager.h"

MainMenuLayout::MainMenuLayout(GUI &gui)
    : Layout("layout_mainmenu", gui) {
    m_properties.constraints.width = RelativeConstraint(1);
    m_properties.constraints.height = RelativeConstraint(1);

    // add elements
    Image* img = new Image {"image_background", ResourceManager::getResource<Texture>(TEXTURE_BACKGROUND_MAIN_MENU)};
    img->properties().constraints.width = RelativeConstraint(1);
    img->properties().constraints.height = RelativeConstraint(1);
    this->addWidget(img);

    // text being (ab)used as button
    Text* t = this->addWidget<Text>("button_quit");
    t->properties().backgroundColor = Color::Gray;
    t->properties().foregroundColor = Color::Emerald;
    t->properties().constraints.x = CenterConstraint();
    t->properties().constraints.y = RelativeConstraint(0.3);
    t->properties().padding.both(20);
    t->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    t->setString("Quit");

    t = this->addWidget<Text>("button_start");
    t->properties().backgroundColor = Color::Gray;
    t->properties().foregroundColor = Color::Emerald;
    t->properties().constraints.x = CenterConstraint();
    t->properties().constraints.y = RelativeConstraint(0.6);
    t->properties().padding.both(20);
    t->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    t->setString("Start");
}
