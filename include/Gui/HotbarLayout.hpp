#pragma once

#include "Layout.hpp"

class Image;

class HotbarLayout : public Layout {
  private:
    Image* m_selector;
    int m_selectionIndex = 0;

  public:

    HotbarLayout(GUI& gui);

    void setSelectionIndex(int index);
    int getSelectionIndex() const;
};