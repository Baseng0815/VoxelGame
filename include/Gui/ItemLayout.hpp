#pragma once
#include "Layout.hpp"

class GUI;

class ItemLayout : public Layout {
  protected:
    void _updateMinimumSize() override;

    float m_slotSpacing = 10.f;

  public:
    ItemLayout(const std::string& id, GUI &gui);

    void setSlotSpacing(float value);
    float getSlotSpacing() const;
};