#include "../../include/Gui/IngameLayerGUI.hpp"

#include "../../include/Gui/DebugLayout.hpp"
#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Gui/Image.hpp"

IngameLayerGUI::IngameLayerGUI()
    : GUI {}
{
    this->addPanel(new DebugLayout(*this));

    Image *img = new Image("image_crosshair", ResourceManager::getResource<Texture>(TEXTURE_CROSSHAIR));
    img->properties().constraints.width = AbsoluteConstraint(32);
    img->properties().constraints.height = AbsoluteConstraint(32);
    img->properties().constraints.x = CenterConstraint();
    img->properties().constraints.y = CenterConstraint();
    this->m_rootLayout->addWidget(img);
}
