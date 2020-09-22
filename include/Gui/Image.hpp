#pragma once

#include "Widget.hpp"
#include "../Rendering/RenderQuad.hpp"

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
        void _updateMinimumSize() override;
        void _updateScreenElements() override;

    public:
        Image(const std::string& id, const Texture *texture = nullptr);

        void setTexture(Texture* texture);
};
