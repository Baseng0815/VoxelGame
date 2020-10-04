#pragma once

#include "../GameData/BlockIds.hpp"
#include "Widget.hpp"
#include "../Rendering/Item.hpp"

class ItemWidget : public Widget {
  private:
    Item m_item;
    const Shader *m_shader;

    BlockId m_block;

    void _draw(const glm::mat4& projection) const override;
    void _updateScreenElements() override;

  public:
    ItemWidget(const std::string &id, const BlockId &block = BlockId::BLOCK_AIR);

    void setBlockId(const BlockId &blockId);
};