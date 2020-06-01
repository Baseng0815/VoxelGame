#include "../include/Text.h"

#include "../include/Font.h"
#include "../include/Layout.h"
#include "../include/Shader.h"

#include <iostream>

void Text::updateTextRenderQuads() {
    float x = m_finalArea.position.x;

    m_charRenderQuads.resize(m_string.size());

    for (int i = 0; i < m_charRenderQuads.size(); i++) {
        const Character& ch = m_font->getCharacter(m_string[i]);
        float xpos = x + ch.bearing.x * m_scale;
        float ypos = m_finalArea.position.y - (ch.size.y - ch.bearing.y) * m_scale;

        float w = ch.size.x * m_scale;
        float h = ch.size.y * m_scale;

        m_charRenderQuads[i].resize(Rectangle(xpos, ypos, w, h), true);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.texture);

        x += (ch.advance >> 6) * m_scale;
    }
}

void Text::updateTextDimensions() {
    m_minWidth = 0;

    for (int i = 0; i < m_charRenderQuads.size(); i++) {
        const Character& ch = m_font->getCharacter(m_string[i]);

        float h = ch.size.y * m_scale;

        if (h > m_minHeight)
            m_minHeight = h;

        m_minWidth += (ch.advance >> 6) * m_scale;
    }
}

void Text::_draw(Shader& shader) const {
    if (m_font == nullptr || m_string.size() == 0)
        return;

    for (int i = 0; i < m_string.size(); i++) {
        const Character& ch = m_font->getCharacter(m_string[i]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.texture);
        m_charRenderQuads[i].render();
    }
}

Text::Text(const std::string& id)
    : Widget(id) {}

void Text::updateArea(const Rectangle& parent) {
    updateTextDimensions();

    m_finalArea = m_properties.constraints.getRect(parent, m_minWidth, m_minHeight);
}

void Text::updateScreenElements() {
    m_renderQuadBackground.resize(m_finalArea);
    updateTextRenderQuads();
}

const std::string& Text::getString() const {
    return m_string;
}

void Text::setString(const std::string& string) {
    m_string = string;
}

const Font* Text::getFont() const {
    return m_font;
}

void Text::setFont(const Font* font) {
    m_font = font;
}
