#pragma once

#include <string>
#include <GL/glew.h>

class Texture {
    private:
        GLuint m_texture;
        int m_width, m_height, m_channels;

    public:
        Texture(const std::string& file);
        ~Texture();

        void bind(int textureUnit);
        int getWidth() const;
        int getHeight() const;
        int getTexture() const;
        int getChannels() const;
};
