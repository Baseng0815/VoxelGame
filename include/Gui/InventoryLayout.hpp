#pragma once

#include "Layout.hpp"
#include <array>

struct InventoryComponent;

class InventoryLayout : public Layout {
  private:
    static std::array<glm::ivec2, 36> m_itemPositions;

  public:
    InventoryLayout(GUI& gui);

    void setInventory(const InventoryComponent& inventory);
};
