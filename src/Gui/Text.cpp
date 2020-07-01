#include "../../include/Gui/Text.h"
#include "../../include/Gui/Layout.h"

#include "../../include/Resources/Font.h"
#include "../../include/Resources/Shader.h"
#include "../../include/Rendering/Window.h"

#include "../../include/ResourceManager.h"

void Text::updateTextRenderQuads() {
    float x = m_widgetArea.position.x;

    m_charRenderQuads.resize(m_string.size());

    for (int i = 0; i < m_charRenderQuads.size(); i++) {
        const Character& ch = m_font->getCharacter(m_string[i]);
        float xpos = x + ch.bearing.x * m_textScale;
        float ypos = m_widgetArea.position.y - (ch.size.y - ch.bearing.y) * m_textScale;

        float w = ch.size.x * m_textScale;
        float h = ch.size.y * m_textScale;

        m_charRenderQuads[i].resize(Rectangle(xpos, ypos, w, h));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.texture);

        x += (ch.advance >> 6) * m_textScale;
    }
}

void Text::updateTextDimensions() {
    m_minWidth = 0;

    for (int i = 0; i < m_charRenderQuads.size(); i++) {
        const Character& ch = m_font->getCharacter(m_string[i]);

        float h = ch.size.y * m_textScale;

        if (h > m_minHeight)
            m_minHeight = h;

        m_minWidth += (ch.advance >> 6) * m_textScale;
    }
}

void Text::_draw(const glm::mat4& projection) const {
    if (m_font == nullptr || m_string.size() == 0)
        return;

    m_textShader->bind();
    m_textShader->upload("projectionMatrix", projection);
    m_textShader->upload("textColor", getForegroundColor());
    for (int i = 0; i < m_string.size(); i++) {
        const Character& ch = m_font->getCharacter(m_string[i]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.texture);
        m_charRenderQuads[i].render();
    }
}

Text::Text(const std::string& id, float textScale)
    : Widget(id), m_textScale(textScale) {
    m_textShader = ResourceManager::getResource<Shader>("shaderText");
}

void Text::updateArea(const Rectangle& parent) {
    updateTextDimensions();

    m_widgetArea = m_properties.constraints.getRect(parent, m_properties, m_minWidth, m_minHeight);
    applyPadding();
}

void Text::updateScreenElements() {
    m_renderQuadBackground.resize(m_finalArea);
    updateTextRenderQuads();
}

void Text::setScale(float scale) {
    m_textScale = scale;
}

float Text::getScale() const {
    return m_textScale;
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
