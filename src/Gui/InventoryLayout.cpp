#include "../../include/Gui/InventoryLayout.hpp"
#include "../../include/Gui/Text.hpp"

#include "../../include/Gui/GUI.hpp"
#include "../../include/Gui/Rectangle.hpp"
#include "../../include/Gui/ItemWidget.hpp"
#include "../../include/Gui/ItemLayout.hpp"

#include "../../include/Color.hpp"
#include "../../include/Components/InventoryComponent.hpp"
#include "../../include/Rendering/Item.hpp"
#include "../../include/Resources/Font.hpp"
#include "../../include/Resources/ResourceManager.hpp"

InventoryLayout::InventoryLayout(GUI &gui)
    : Layout{"layout_inventory", gui},
      m_title{new Text("inventory_title", 1.5f)},
      m_itemLayout{new ItemLayout{"inventory_items", gui}} {
    m_properties.constraints.height = RelativeConstraint(0.5f);
    m_properties.constraints.width = RelativeConstraint(0.5f);
    m_properties.constraints.x = RelativeConstraint(0.25f);
    m_properties.constraints.y = RelativeConstraint(0.25f);
    m_properties.padding.both(10);
    m_properties.isVisible = false;

    m_properties.backgroundColor = Color{21, 67, 96, 128};
    setStackMode(StackMode::STACK_VERTICAL);

    m_title->properties().constraints.x = CenterConstraint();
    m_title->properties().constraints.y = AbsoluteConstraint(0);

    m_title->setString("Inventory");
    m_title->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_title);
        
    m_itemLayout->properties().backgroundColor = Color::Azure;
    m_itemLayout->properties().margin.top = 10;
    this->addWidget(m_itemLayout);
}

void InventoryLayout::setInventory(const InventoryComponent &inventory) {
    m_itemLayout->clearWidgets();

    for (int i = 0; i < inventory.slotsCount; i++) {
        if (inventory.slots[i].second != 0) {
            Widget* slot = new ItemWidget{"slot_" + std::to_string(i), inventory.slots[i].first};
            slot->properties().constraints.x = AbsoluteConstraint(0);
            slot->properties().constraints.y = AbsoluteConstraint(0);
            slot->properties().margin.both(10);

            m_itemLayout->addWidget(slot);
        }
    }       

    m_itemLayout->_invalidate();
}
