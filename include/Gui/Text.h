#pragma once

#include "Widget.h"
#include "../Rendering/RenderQuad.h"

#include <vector>

class Font;
class Layout;
class Shader;

class Text : public Widget {
    private:
        std::vector<RenderQuad> m_charRenderQuads;

        std::string m_string = "";

        const Font* m_font = nullptr;
        // TODO maybe make static or access every frame
        Shader* m_textShader;

        float m_textScale;
        void updateTextRenderQuads();

        // update minWidth and minHeight
        void updateTextDimensions();
        void _draw(const glm::mat4& projection) const override;

    public:
        Text(const std::string& id, float textScale = 1.0f);

        void updateArea(const Rectangle& parent) override;
        void updateScreenElements() override;

        void setScale(float scale);
        float getScale() const;
        const std::string& getString() const;
        void setString(const std::string& str);
        const Font* getFont() const;
        void setFont(const Font* font);
};
