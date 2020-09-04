#pragma once

#include <string>
#include <GL/glew.h>
#include <array>

#include "Resource.h"

class Texture : public Resource {
    private:
        GLuint m_texture;
        int m_width, m_height, m_channels;

        void release() override;

    public:
        // single texture
        Texture(const std::string& file);

        // cube map
        Texture(const std::array<std::string, 6> &files);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture &operator=(const Texture&) = delete;
        Texture(Texture&&) noexcept;
        Texture &operator=(Texture&&) noexcept;

        void bind(GLenum textureUnit = 0, GLenum target = GL_TEXTURE_2D) const;
        int getWidth() const;
        int getHeight() const;
        int getTexture() const;
        int getChannels() const;
};
