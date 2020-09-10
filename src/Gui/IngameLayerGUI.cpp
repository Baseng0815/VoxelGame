#include "../../include/Gui/IngameLayerGUI.h"

#include "../../include/Gui/DebugLayout.h"
#include "../../include/Resources/ResourceManager.h"
#include "../../include/Gui/Image.h"

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
