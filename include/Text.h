#pragma once

#include "Widget.h"
#include "RenderQuad.h"

#include <vector>

class Font;
class Layout;
class Shader;

class Text : public Widget {
    private:
        std::vector<RenderQuad> m_charRenderQuads;
        glm::mat4 m_renderQuadTranslation;
        float m_textWidth = 0, m_textHeight = 0;

        std::string m_string = "";

        const Font* m_font = nullptr;

        void updateRenderQuads();
        void _draw(Shader& shader) const override;

    public:
        Text(const std::string& id, Layout* parent);

        virtual void resize(Rectangle parent = Rectangle()) override;

        const std::string& getString() const;
        void setString(const std::string& str);
        const Font* getFont() const;
        void setFont(const Font* font);
};
