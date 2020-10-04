#include "../../include/Gui/ItemLayout.hpp"

#include "../../include/Gui/GUI.hpp"

ItemLayout::ItemLayout(const std::string& id, GUI &gui)
    : Layout{id, gui} {
    setStackMode(STACK_HORIZONTAL);    
}

void ItemLayout::_updateMinimumSize() {
    // TODO: Wrap
    m_minSize = glm::vec2{m_widgets.size() * (48 + m_slotSpacing) - m_slotSpacing, 48};
}
