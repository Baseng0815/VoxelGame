#include "../../include/Gui/Image.h"

#include "../../include/ResourceManager.h"
#include "../../include/Resources/Shader.h"
#include "../../include/Resources/Texture.h"

void Image::updateImageDimensions() {
    m_minWidth = m_texture->getWidth();
    m_minHeight = m_texture->getHeight();
}

void Image::_draw(const glm::mat4& projection) const {
    if (m_texture) {
        m_texturedQuadShader->bind();
        m_texturedQuadShader->upload("projectionMatrix", projection);
        m_texture->bind(0);
        m_renderQuad.render();
    }
}

Image::Image(const std::string& id, GUI *gui, Texture* texture)
    : Widget(id, gui), m_texture(texture) {
    m_texturedQuadShader = ResourceManager::getResource<Shader>("shaderTexturedQuad");
}

void Image::updateArea(const Rectangle& parent) {
    updateImageDimensions();

    m_widgetArea = m_properties.constraints.getRect(parent, m_properties, m_minWidth, m_minHeight);
    applyPadding();
}

void Image::updateScreenElements() {
    m_renderQuadBackground.resize(m_finalArea);
    m_renderQuad.resize(m_widgetArea);
}
