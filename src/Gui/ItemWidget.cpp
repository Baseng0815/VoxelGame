#include "../../include/Gui/ItemWidget.hpp"

#include "../../include/Rendering/Item.hpp"
#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Shader.hpp"
#include "../../include/Resources/Texture.hpp"

ItemWidget::ItemWidget(const std::string &id, const BlockId &block)
    : Widget{id}, m_block{block},
      m_shader{ResourceManager::getResource<Shader>(SHADER_TEXTURE_QUAD)} {
    m_minSize = glm::vec2{48.0f};
}

void ItemWidget::_draw(const glm::mat4 &projection) const {
    if (m_block != BlockId::BLOCK_AIR) {
        const Texture *texture = ResourceManager::getResource<Texture>(TEXTURE_ATLAS);
        m_shader->bind();

        if (!m_shader->uniformsSet()) {
            m_shader->upload("projectionMatrix", projection);
            m_shader->setUniformState(true);
        }

        texture->bind(GL_TEXTURE0);
        m_item.render();
    }
}

void ItemWidget::_updateScreenElements() {
    m_item.resize(m_innerArea, m_block);
}

void ItemWidget::setBlockId(const BlockId &block) {
    m_block = block;
    _invalidate();
}