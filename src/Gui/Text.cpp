#include "../../include/Gui/GUI.h"
#include "../../include/Gui/Text.h"

#include "../../include/Resources/Font.h"
#include "../../include/Resources/Shader.h"
#include "../../include/Rendering/Window.h"

#include "../../include/Resources/ResourceManager.h"

#include <iostream>

void Text::_updateScreenElements()
{
    float x = m_innerArea.position.x;

    // TODO find out why the hell just resizing the quads does NOT work (text becomes white)
    m_charRenderQuads.clear();
    m_charRenderQuads.resize(m_string.size());

    for (int i = 0; i < m_charRenderQuads.size(); i++) {
        const Character& ch = m_font->getCharacter(m_string[i]);
        float xpos = x + ch.bearing.x * m_textScale;
        float ypos = m_innerArea.position.y - (ch.size.y - ch.bearing.y) * m_textScale;

        float w = ch.size.x * m_textScale;
        float h = ch.size.y * m_textScale;

        // adjustments for relatively sized text
        if (m_properties.constraints.width.getType() != ConstraintType::CONSTRAINT_MATCH) {
            float scale = m_innerArea.size.x / m_minSize.x;
            w *= scale;
            x += (ch.advance >> 6) * m_textScale * scale;
        } else {
            x += (ch.advance >> 6) * m_textScale;
        }

        if (m_properties.constraints.height.getType() != ConstraintType::CONSTRAINT_MATCH) {
            float scale = m_innerArea.size.y / m_minSize.y;
            h *= scale;
        }

        m_charRenderQuads[i].resize(Rectangle(xpos, ypos, w, h));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.texture);

    }
}

void Text::_updateMinimumSize()
{
    m_minSize.x = 0;

    for (char c : m_string) {
        const Character &ch = m_font->getCharacter(c);

        float h = ch.size.y * m_textScale;

        if (h > m_minSize.y)
            m_minSize.y = h;

        m_minSize.x += (ch.advance >> 6) * m_textScale;
    }
}

void Text::_draw(const glm::mat4& projection) const {
    if (m_font == nullptr || m_string.size() == 0)
        return;

    m_textShader->bind();
    if (!m_textShader->uniformsSet()) {
        m_textShader->upload("projectionMatrix", projection);
        m_textShader->setUniformState(true);
    }
    m_textShader->upload("textColor", getForegroundColor());
    for (int i = 0; i < m_string.size(); i++) {
        const Character& ch = m_font->getCharacter(m_string[i]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.texture);
        m_charRenderQuads[i].render();
    }
}

Text::Text(const std::string& id, float textScale)
    : Widget {id}, m_textScale {textScale},
    m_textShader {ResourceManager::getResource<Shader>(SHADER_TEXT)}
{}

void Text::setScale(float scale) {
    m_textScale = scale;
}

float Text::getScale() const
{ return m_textScale; }

const std::string& Text::getString() const
{ return m_string; }

void Text::setString(const std::string& string)
{
    m_string = string;
    _invalidate();
}

const Font* Text::getFont() const
{ return m_font; }

void Text::setFont(const Font* font)
{
    m_font = font;
    _invalidate();
}
