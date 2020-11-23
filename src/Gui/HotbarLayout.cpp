#include "../../include/Gui/HotbarLayout.hpp"

#include "../../include/Components/InventoryComponent.hpp"
#include "../../include/Gui/Image.hpp"
#include "../../include/Gui/ItemLayout.hpp"
#include "../../include/Resources/ResourceIDs.hpp"
#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Texture.hpp"

HotbarLayout::HotbarLayout(GUI& gui)
    : Layout{"hotbar_layout", gui}
{

    Image* hotbar = new Image("image_hotbar", ResourceManager::getResource<Texture>(TEXTURE_GUI_HOTBAR));
    hotbar->properties().constraints.width = RelativeConstraint(1);
    hotbar->properties().constraints.height = RelativeConstraint(1);
    hotbar->properties().constraints.x = CenterConstraint();
    hotbar->properties().constraints.y = CenterConstraint();
    this->addWidget(hotbar);

    m_properties.constraints.height = AbsoluteConstraint(74);
    m_properties.constraints.width = AbsoluteConstraint(666);
    m_properties.constraints.x = CenterConstraint();
    m_properties.constraints.y = AbsoluteConstraint(0);

    int offset = 5;
    int width = 74;

    for (int i = 0; i < 9; i++) {
        ItemLayout* item = new ItemLayout{"hotbar_slot" + i, gui};
        item->properties().constraints.x = AbsoluteConstraint(offset + i * width);
        item->properties().constraints.y = CenterConstraint();
        item->properties().constraints.height = AbsoluteConstraint(64);
        item->properties().constraints.width = AbsoluteConstraint(64);

        addWidget(item);
        m_items[i] = item;
    }

    m_selector = new Image("hotbar_selector", ResourceManager::getResource<Texture>(TEXTURE_GUI_HOTBAR_SELECTOR));
    m_selector->properties().constraints.x = AbsoluteConstraint(0);
    m_selector->properties().constraints.y = AbsoluteConstraint(0);
    m_selector->properties().constraints.height = AbsoluteConstraint(74);
    m_selector->properties().constraints.width = AbsoluteConstraint(74);

    addWidget(m_selector);
}

void HotbarLayout::setSelectionIndex(int index)
{
    m_selectionIndex = index;

    m_selector->properties().constraints.x = AbsoluteConstraint(m_selectionIndex * 74);
    m_selector->_invalidate();
}

int HotbarLayout::getSelectionIndex() const
{
    return m_selectionIndex;
}

void HotbarLayout::updateItems(const InventoryComponent &inventory)
{
    for (int i = 0; i < 9; i++) {
        auto& [block, count] = inventory.slots[i];
        m_items[i]->setData(block, count);
    }
}
