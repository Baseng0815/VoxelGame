#include "../../include/Gui/InventoryLayout.hpp"

#include "../../include/Components/ItemComponent.hpp"
#include "../../include/Gui/Image.hpp"
#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Texture.hpp"

// clang-format off
std::array<glm::ivec2, 36> InventoryLayout::m_itemPositions = {
    glm::ivec2{25, 25}, glm::ivec2{99, 25}, glm::ivec2{173, 25}, glm::ivec2{247, 25}, glm::ivec2{321, 25}, glm::ivec2{395, 25}, glm::ivec2{469, 25}, glm::ivec2{543, 25}, glm::ivec2{617, 25},
    glm::ivec2{25, 311}, glm::ivec2{99, 311}, glm::ivec2{173, 311}, glm::ivec2{247, 311}, glm::ivec2{321, 311}, glm::ivec2{395, 311}, glm::ivec2{469, 311}, glm::ivec2{543, 311}, glm::ivec2{617, 311},
    glm::ivec2{25, 237}, glm::ivec2{99, 237}, glm::ivec2{173, 237}, glm::ivec2{247, 237}, glm::ivec2{321, 237}, glm::ivec2{395, 237}, glm::ivec2{469, 237}, glm::ivec2{543, 237}, glm::ivec2{617, 237},
    glm::ivec2{25, 163}, glm::ivec2{99, 163}, glm::ivec2{173, 163}, glm::ivec2{247, 163}, glm::ivec2{321, 163}, glm::ivec2{395, 163}, glm::ivec2{469, 163}, glm::ivec2{543, 163}, glm::ivec2{617, 163}
};
// clang-format on

InventoryLayout::InventoryLayout(GUI & gui)
    : Layout{"inventory_layout", gui} {
    m_properties.constraints.x = CenterConstraint();
    m_properties.constraints.y = CenterConstraint();
    m_properties.constraints.width = AbsoluteConstraint(726);
    m_properties.constraints.height = AbsoluteConstraint(400);

    Image* background = new Image("inventory_background", ResourceManager::getResource<Texture>(TEXTURE_GUI_INVENTORY));
    background->properties().constraints.x = CenterConstraint();
    background->properties().constraints.y = CenterConstraint();
    background->properties().constraints.width = AbsoluteConstraint(726);
    background->properties().constraints.height = AbsoluteConstraint(400);
    addWidget(background);

    m_properties.isVisible = false;
}
