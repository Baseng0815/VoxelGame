#include "../../include/Gui/InventoryLayout.hpp"

#include "../../include/Components/InventoryComponent.hpp"
#include "../../include/Gui/Image.hpp"
#include "../../include/Gui/ItemLayout.hpp"
#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Texture.hpp"

InventoryLayout::InventoryLayout(GUI& gui)
    : Layout{"inventory_layout", gui} {
    m_properties.constraints.x = CenterConstraint();
    m_properties.constraints.y = CenterConstraint();
    m_properties.constraints.height = AbsoluteConstraint(400);
    m_properties.constraints.width = AbsoluteConstraint(726);

    Image* background = new Image("inventory_background", ResourceManager::getResource<Texture>(TEXTURE_GUI_INVENTORY));
    background->properties().constraints.x = AbsoluteConstraint(0);
    background->properties().constraints.y = AbsoluteConstraint(0);
    background->properties().constraints.width = RelativeConstraint(1);
    background->properties().constraints.height = RelativeConstraint(1);
    addWidget(background);

    for (int i = 0; i < 36; i++) {
        // TODO: Fix widget id
        // ItemLayout* item = new ItemLayout{"inventory_slot_" + i, gui};
        ItemLayout* item = new ItemLayout{"inventory_layout?fkdjlkgjhlgj_slot_" + i, gui};
        glm::ivec2 position = m_itemPositions[i];
        item->properties().constraints.x = AbsoluteConstraint(position.x);
        item->properties().constraints.y = AbsoluteConstraint(position.y);        

        addWidget(item);        
        m_slots[i] = item;        
    }

    m_properties.isVisible = false;
}

void InventoryLayout::setInventory(const InventoryComponent& inventory) {
    for (int i = 0; i < m_itemPositions.size(); i++) {
        auto& [block, count] = inventory.slots[i];
        m_slots[i]->setData(block, count);        
    }
}
