#include "../../include/Gui/Image.h"

#include "../../include/Resources/ResourceManager.h"
#include "../../include/Resources/Shader.h"
#include "../../include/Resources/Texture.h"

void Image::_draw(const glm::mat4& projection) const {
    if (m_texture) {
        m_texturedQuadShader->bind();
        m_texturedQuadShader->upload("projectionMatrix", projection);
        m_texture->bind(0);
        m_renderQuad.render();
    }
}

void Image::_updateMinimumSize() {
    m_minSize.x = m_texture->getWidth();
    m_minSize.y = m_texture->getHeight();
}

void Image::_updateScreenElements() {
    m_renderQuad.resize(m_innerArea);
}

Image::Image(const std::string& id, const Texture *texture)
    : Widget {id}, m_texture {texture},
    m_texturedQuadShader {ResourceManager::getResource<Shader>(SHADER_TEXTURE_QUAD)}
{}
