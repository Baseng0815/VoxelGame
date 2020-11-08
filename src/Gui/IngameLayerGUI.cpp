#include "../../include/Gui/IngameLayerGUI.hpp"

#include "../../include/Gui/DebugLayout.hpp"
#include "../../include/Gui/Image.hpp"
#include "../../include/Gui/InventoryLayout.hpp"
#include "../../include/Gui/HotbarLayout.hpp"
#include "../../include/Resources/ResourceManager.hpp"

IngameLayerGUI::IngameLayerGUI()
    : GUI{} {
    this->addPanel(new DebugLayout(*this));
    this->addPanel(new InventoryLayout(*this));
    this->addPanel(new HotbarLayout(*this));

    Image* img = new Image("image_crosshair", ResourceManager::getResource<Texture>(TEXTURE_CROSSHAIR));
    img->properties().constraints.width = AbsoluteConstraint(32);
    img->properties().constraints.height = AbsoluteConstraint(32);
    img->properties().constraints.x = CenterConstraint();
    img->properties().constraints.y = CenterConstraint();
    this->m_rootLayout->addWidget(img);    
}
