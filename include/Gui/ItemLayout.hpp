#pragma once

#include "Layout.hpp"
#include "../GameData/BlockIds.hpp"

class ItemWidget;
class Text;

class ItemLayout : public Layout {
  private:
    ItemWidget* m_item;
    Text* m_text;

  public:
    ItemLayout(const std::string& id, GUI& gui);

    void setData(const BlockId& block, const int count);
};
