#include "../../include/Gui/ItemLayout.hpp"

#include "../../include/Gui/ItemWidget.hpp"
#include "../../include/Gui/Text.hpp"
#include "../../include/Resources/ResourceManager.hpp"

ItemLayout::ItemLayout(const std::string& id, GUI& gui)
    : Layout{id, gui} {
    m_item = new ItemWidget{"item_" + id, BlockId::BLOCK_STONE_BRICKS};
    m_item->properties().padding.both(5);
    addWidget(m_item);

    m_text = new Text{"text_" + id};
    m_text->setString("0");
    m_text->properties().constraints.x = AbsoluteConstraint(2);
    m_text->properties().constraints.y = AbsoluteConstraint(2);
    m_text->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    m_text->properties().isVisible = false;
    addWidget(m_text);
}

void ItemLayout::setData(const BlockId& block, const int count) {
    m_item->setBlockId(block);
    m_text->setString(std::to_string(count));

    m_item->properties().isVisible = (count != 0);
    m_text->properties().isVisible = (count != 0);

    m_item->_invalidate();
    m_text->_invalidate();
}
