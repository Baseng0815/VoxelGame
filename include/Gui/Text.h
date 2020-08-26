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

        const Font *m_font = nullptr;
        // TODO maybe make static or access every frame
        const Shader *m_textShader;

        float m_textScale;

        void _draw(const glm::mat4& projection) const override;
        void _updateScreenElements() override;
        void _updateMinimumSize() override;

    public:
        Text(const std::string &id, GUI *gui, float textScale = 1.0f);

        void setScale(float scale);
        float getScale() const;
        const std::string& getString() const;

        void setString(const std::string &str);
        const Font *getFont() const;
        void setFont(const Font *font);
};
