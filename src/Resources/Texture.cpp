#include "../../include/Resources/Texture.h"

#if defined(WIN32)
#include <SOIL.h>
#else
#include <SOIL/SOIL.h>
#endif

#include <iostream>

#include "../../include/Configuration.h"

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

void Texture::free() {
    glDeleteTextures(1, &m_texture);
}

void Texture::bind(int textureUnit) {
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
