#include "../include/Text.h"

#include "../include/Font.h"
#include "../include/Layout.h"
#include "../include/Shader.h"

#include <iostream>

void Text::updateRenderQuads() {
    float scale = 0.8f;
    float x = m_area.position.x;

    m_charRenderQuads.resize(m_string.size());

    for (int i = 0; i < m_charRenderQuads.size(); i++) {
        const Character& ch = m_font->getCharacter(m_string[i]);
        float xpos = x + ch.bearing.x * scale;
        float ypos = m_area.position.y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        if (h > m_textHeight)
            m_textHeight = h;

        m_charRenderQuads[i].resize(Rectangle(xpos, ypos, w, h), true);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.texture);

        x += (ch.advance >> 6) * scale;
    }

    m_textWidth = x - m_area.position.x;
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

Text::Text(const std::string& id, Layout* parent)
    : Widget(id, parent) {}

void Text::resize(Rectangle parent) {
    m_area = m_properties.constraints.getRect(m_parent->getArea());

    if (m_properties.constraints.width.getType() == CONSTRAINT_MATCH)
        m_area.size.x = m_textWidth;
    if (m_properties.constraints.height.getType() == CONSTRAINT_MATCH)
        m_area.size.y = m_textHeight;

    m_renderQuadBackground.resize(m_area);
    updateRenderQuads();
}

const std::string& Text::getString() const {
    return m_string;
}

void Text::setString(const std::string& string) {
    m_string = string;
    updateRenderQuads();
}

const Font* Text::getFont() const {
    return m_font;
}

void Text::setFont(const Font* font) {
    m_font = font;
}
