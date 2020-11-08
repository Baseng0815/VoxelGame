#pragma once

#include "Layout.hpp"

class ItemWidget;

class ItemLayout : public Layout {
  private:
    ItemWidget* m_item;

  public:
    ItemLayout(const std::string& id, GUI& gui);
};
