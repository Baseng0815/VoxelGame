#include "../../include/Gui/Image.hpp"

#include "../../include/Resources/ResourceManager.hpp"
#include "../../include/Resources/Shader.hpp"
#include "../../include/Resources/Texture.hpp"

#include <iostream>
void Image::_draw(const glm::mat4& projection) const {
    if (m_texture) {
        m_texturedQuadShader->bind();
        if (!m_texturedQuadShader->uniformsSet()) {
            m_texturedQuadShader->upload("projectionMatrix", projection);
            m_texturedQuadShader->setUniformState(true);
        }
        m_texture->bind(GL_TEXTURE0);
        m_renderQuad.render();
    }
}

void Image::_updateMinimumSize() {
    m_minSize.x = m_texture->getWidth();
    m_minSize.y = m_texture->getHeight();
}

void Image::_updateScreenElements() {
    m_renderQuad.resize(m_outerArea);
}

Image::Image(const std::string& id, const Texture *texture)
    : Widget {id}, m_texture {texture},
    m_texturedQuadShader {ResourceManager::getResource<Shader>(SHADER_TEXTURE_QUAD)}
{}
