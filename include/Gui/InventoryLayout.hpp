#pragma once
#include "Layout.hpp"

#include "../../include/GameData/BlockIds.hpp"
#include <vector>

class GUI;
struct InventoryComponent;
class Text;

class InventoryLayout : public Layout {  
  private:
    struct InventorySlot {
        int index;
        BlockId block;
        int count;
    };

    std::vector<InventorySlot> items;

    Text *m_title;
    Layout *firstRow, *secndRow, *thirdRow, *fourthRow;


  public:
    InventoryLayout(GUI &gui);    
};