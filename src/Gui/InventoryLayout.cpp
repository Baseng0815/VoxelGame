#include "../../include/Gui/InventoryLayout.hpp"

#include "../../include/Components/InventoryComponent.hpp"
#include "../../include/Events/Event.hpp"
#include "../../include/Gui/Image.hpp"
#include "../../include/Gui/ItemLayout.hpp"
#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Texture.hpp"

void InventoryLayout::onCursorMove(const CursorEvent& e) {
    if (!m_properties.isVisible) {
        m_selectedSlot = -1;
        return;
    }

    int slot = getSlot(glm::vec2{e.x, e.y} - m_innerArea.position);
    m_selectedSlot = slot;
    if (slot >= 0) {
        glm::ivec2 position = m_itemPositions[slot];
        m_slotSelector->properties().constraints.x = AbsoluteConstraint(position.x);
        m_slotSelector->properties().constraints.y = AbsoluteConstraint(position.y);

        m_slotSelector->properties().isVisible = true;
        m_slotSelector->_invalidate();
    }
    else {
        m_slotSelector->properties().isVisible = false;
        m_slotSelector->_invalidate();
    }    
}

void InventoryLayout::onMouseButton(const MouseButtonEvent& e) {
    if (m_playerInventory && m_selectedSlot >= 0) {
        if (m_dragContext.hasData) {
            BlockId item;
            int count;
            std::tie(item, count) = m_dragContext.data;

            if (m_playerInventory->slots[m_selectedSlot].second > 0) {
                m_dragContext.data = std::pair<BlockId, int>{
                    m_playerInventory->slots[m_selectedSlot].first,
                    m_playerInventory->slots[m_selectedSlot].second};
                                
            }
            else {
                m_dragContext.hasData = false;                
            }

            m_playerInventory->slots[m_selectedSlot].first = item;
            m_playerInventory->slots[m_selectedSlot].second = count;
            m_slots[m_selectedSlot]->setData(item, count);
            m_slots[m_selectedSlot]->_invalidate();
        }
        else {
            if (m_playerInventory->slots[m_selectedSlot].second > 0) {
                m_dragContext.data = std::pair<BlockId, int>{
                    m_playerInventory->slots[m_selectedSlot].first,
                    m_playerInventory->slots[m_selectedSlot].second};

                m_dragContext.hasData = true;
                
                m_playerInventory->slots[m_selectedSlot] = std::pair<BlockId, int>{BlockId::BLOCK_AIR, 0};
                m_slots[m_selectedSlot]->setData(BlockId::BLOCK_AIR, 0);
                m_slots[m_selectedSlot]->_invalidate();
            }
        }
    }
}

int InventoryLayout::getSlot(const glm::vec2& cursorPosition) {
    glm::vec2 slotSize = glm::vec2{64};
    for (int i = 0; i < 36; i++) {
        glm::vec2 p = m_itemPositions[i];
        glm::vec2 q = p + slotSize;

        if (cursorPosition.x < q.x && cursorPosition.x >= p.x && cursorPosition.y < q.y && cursorPosition.y >= p.y) {
            return i;
        }
    }

    return -1;
}

InventoryLayout::InventoryLayout(GUI& gui)
    : Layout{"inventory_layout", gui} {
    m_properties.constraints.x = CenterConstraint();
    m_properties.constraints.y = CenterConstraint();
    m_properties.constraints.height = AbsoluteConstraint(400);
    m_properties.constraints.width = AbsoluteConstraint(726);

    onMoveHandle = onMove.subscribe([&](const CursorEvent& e) { onCursorMove(e); });
    onMouseButtonHandle = onPress.subscribe([&](const MouseButtonEvent& e) { onMouseButton(e); });

    Image* background = new Image("inventory_background", ResourceManager::getResource<Texture>(TEXTURE_GUI_INVENTORY));
    background->properties().constraints.x = AbsoluteConstraint(0);
    background->properties().constraints.y = AbsoluteConstraint(0);
    background->properties().constraints.width = RelativeConstraint(1);
    background->properties().constraints.height = RelativeConstraint(1);
    addWidget(background);

    m_slotSelector = new Image("inventory_layout_selector", ResourceManager::getResource<Texture>(TEXTURE_GUI_SLOT_SELECTOR));
    m_slotSelector->properties().constraints.width = AbsoluteConstraint(64);
    m_slotSelector->properties().constraints.height = AbsoluteConstraint(64);
    m_slotSelector->properties().isVisible = false;
    addWidget(m_slotSelector);

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

void InventoryLayout::setInventory(InventoryComponent& inventory) {
    for (int i = 0; i < m_itemPositions.size(); i++) {
        auto& [block, count] = inventory.slots[i];
        m_slots[i]->setData(block, count);
    }

    m_playerInventory = &inventory;
}
