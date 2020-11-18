#pragma once

#include "Layout.hpp"

#include <array>
#include <entt/entt.hpp>

class Image;
class ItemLayout;
struct InventoryComponent;

class HotbarLayout : public Layout {
  private:
    Image* m_selector;
    int m_selectionIndex = 0;
    std::array<ItemLayout*, 9> m_items;
    entt::entity m_player;

  public:
    HotbarLayout(GUI& gui);

    void setSelectionIndex(int index);
    int getSelectionIndex() const;    

    void updateItems(const InventoryComponent& inventory);
};