#pragma once

#include <string>
#include <GL/glew.h>

#include "Resource.h"

class Texture : public Resource {
    private:
        GLuint m_texture;
        int m_width, m_height, m_channels;

        void release() override;

    public:
        Texture(const std::string& file);
        ~Texture();

        Texture(Texture&&) noexcept;
        Texture &operator=(Texture&&) noexcept;

        void bind(int textureUnit) const;
        int getWidth() const;
        int getHeight() const;
        int getTexture() const;
        int getChannels() const;
};
