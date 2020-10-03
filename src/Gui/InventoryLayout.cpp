#include "../../include/Gui/InventoryLayout.hpp"
#include "../../include/Gui/Text.hpp"

#include "../../include/Gui/GUI.hpp"
#include "../../include/Gui/Rectangle.hpp"

#include "../../include/Color.hpp"
#include "../../include/Components/InventoryComponent.hpp"
#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Font.hpp"

InventoryLayout::InventoryLayout(GUI &gui)
    : Layout{"layout_inventory", gui},
      m_title{new Text("inventory_title", 1.5f)} {
    m_properties.constraints.height = RelativeConstraint(0.5f);
    m_properties.constraints.width = RelativeConstraint(0.5f);
    m_properties.constraints.x = RelativeConstraint(0.25f);
    m_properties.constraints.y = RelativeConstraint(0.25f);
    m_properties.padding.both(10);
    m_properties.isVisible = false;

    m_properties.backgroundColor = Color{21, 67, 96, 128};
    setStackMode(StackMode::STACK_HORIZONTAL);

    // for (int i = 0; i < inventory.slotsCount; i++) {
    //     if (inventory.slots[i].second != 0) {
    //         items.emplace_back(InventorySlot{i, inventory.slots[i].first, inventory.slots[i].second});
    //     }
    // }

    m_title->properties().constraints.x = CenterConstraint();
    m_title->properties().constraints.y = AbsoluteConstraint(10);

    m_title->setString("Inventory");
    m_title->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_title);
}
