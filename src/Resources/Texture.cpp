#include "../../include/Resources/Texture.h"

#include <SOIL/SOIL.h>

#include <iostream>

#include "../../include/Configuration.h"

void Texture::release()
{
    glDeleteTextures(1, &m_texture);
}

Texture::Texture(const std::string& file) {
    std::cout << "loading texture " << file << std::endl;
    glGenTextures(1, &m_texture);

    unsigned char* data = SOIL_load_image((Configuration::getStringValue("ResourceBasePath") + file).c_str(), &m_width, &m_height, &m_channels, SOIL_LOAD_RGBA);
    if (!data)
        std::cout << "failed to load texture" << file << std::endl;

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    SOIL_free_image_data(data);
}

Texture::~Texture()
{
    release();
}

Texture::Texture(Texture &&other) noexcept
    : m_texture(other.m_texture), m_width(other.m_width),
    m_height(other.m_height), m_channels(other.m_channels)
{
    other.m_texture = 0;
    other.m_height = 0;
    other.m_width = 0;
    other.m_channels = 0;
}

Texture &Texture::operator=(Texture &&other) noexcept
{
    if (this != &other) {
        release();
        std::swap(m_texture, other.m_texture);
        std::swap(m_width, other.m_width);
        std::swap(m_height, other.m_height);
        std::swap(m_channels, other.m_channels);
    }

    return *this;
}

void Texture::bind(int textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

int Texture::getWidth() const {
    return m_width;
}

int Texture::getHeight() const {
    return m_height;
}

int Texture::getTexture() const {
    return m_texture;
}

int Texture::getChannels() const {
    return m_channels;
}
