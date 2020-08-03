#pragma once

#include "Widget.h"
#include "../Rendering/RenderQuad.h"

class Layout;
class Shader;
class Texture;

class Image : public Widget {
    private:
        const Texture *m_texture;
        const Shader *m_texturedQuadShader;
        RenderQuad m_renderQuad;

        // update minWidth and minHeight
        void updateImageDimensions();
        void _draw(const glm::mat4& projection) const override;

    public:
        Image(const std::string& id, GUI *gui, const Texture *texture = nullptr);

        void updateArea(const Rectangle& parent) override;
        void updateScreenElements() override;

        void setTexture(Texture* texture);
};
