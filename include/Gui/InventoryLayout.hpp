#pragma once
#include "Layout.hpp"

#include "../../include/GameData/BlockIds.hpp"
#include <vector>

class GUI;
struct InventoryComponent;
class Text;
class ItemLayout;

class InventoryLayout : public Layout {  
  private:
    struct InventorySlot {
        int index;
        BlockId block;
        int count;
    };    

    Text *m_title;
    ItemLayout *m_itemLayout;

  public:
    InventoryLayout(GUI &gui);

    void setInventory(const InventoryComponent &inventory);
};