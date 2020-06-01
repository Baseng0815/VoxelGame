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

        std::string m_string = "";

        const Font* m_font = nullptr;

        // TODO create text properties
        float m_scale = 0.3f;

        void updateTextRenderQuads();

        // updates minWidth and minHeight without recreating any quads
        void updateTextDimensions();
        void _draw(Shader& shader) const override;

    public:
        Text(const std::string& id);

        void updateArea(const Rectangle& parent) override;
        virtual void updateScreenElements() override;

        const std::string& getString() const;
        void setString(const std::string& str);
        const Font* getFont() const;
        void setFont(const Font* font);
};
