#pragma once

#include "Layout.hpp"
#include <array>
#include <glm/glm.hpp>

struct InventoryComponent;
class ItemLayout;

class InventoryLayout : public Layout {
  private:
    // clang-format off
    static constexpr std::array<glm::ivec2, 36> m_itemPositions = {
        glm::ivec2{25, 25}, glm::ivec2{99, 25}, glm::ivec2{173, 25}, glm::ivec2{247, 25}, glm::ivec2{321, 25}, glm::ivec2{395, 25}, glm::ivec2{469, 25}, glm::ivec2{543, 25}, glm::ivec2{617, 25},
        glm::ivec2{25, 311}, glm::ivec2{99, 311}, glm::ivec2{173, 311}, glm::ivec2{247, 311}, glm::ivec2{321, 311}, glm::ivec2{395, 311}, glm::ivec2{469, 311}, glm::ivec2{543, 311}, glm::ivec2{617, 311},
        glm::ivec2{25, 237}, glm::ivec2{99, 237}, glm::ivec2{173, 237}, glm::ivec2{247, 237}, glm::ivec2{321, 237}, glm::ivec2{395, 237}, glm::ivec2{469, 237}, glm::ivec2{543, 237}, glm::ivec2{617, 237},
        glm::ivec2{25, 163}, glm::ivec2{99, 163}, glm::ivec2{173, 163}, glm::ivec2{247, 163}, glm::ivec2{321, 163}, glm::ivec2{395, 163}, glm::ivec2{469, 163}, glm::ivec2{543, 163}, glm::ivec2{617, 163}
    };
    // clang-format on

    std::array<ItemLayout*, 36> m_slots;

  public:
    InventoryLayout(GUI& gui);

    void setInventory(const InventoryComponent& inventory);
};
