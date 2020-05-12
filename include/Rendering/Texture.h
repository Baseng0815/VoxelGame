#pragma once

#include <GL/glew.h>

#include "../../include/Resource.h"

class Texture : public Resource {
    private:
        GLuint m_texture;
        int m_width, m_height, m_channels;

    public:
        Texture(const std::string& path);
        ~Texture();

        void bind(int textureUnit);
        int getWidth() const;
        int getHeight() const;
        int getTexture() const;
        int getChannels() const;
};
