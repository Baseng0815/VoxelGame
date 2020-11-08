#include "../../include/Gui/ItemLayout.hpp"

#include "../../include/Gui/ItemWidget.hpp"

ItemLayout::ItemLayout(const std::string& id, GUI& gui)
    : Layout{id, gui} {
    m_item = new ItemWidget("item_" + id, BlockId::BLOCK_STONE);        
    m_item->properties().constraints.y = CenterConstraint();
    m_item->properties().padding.both(5);

    addWidget(m_item);
}