#include "../../include/Resources/Font.h"

#include <iostream>
#include <freetype2/ft2build.h>
#include <freetype2/freetype/freetype.h>

#include "../../include/Configuration.h"
#include "../../include/ResourceManager.h"

Font::Font(const std::string& file) {
    std::cout << "loading font " << file << std::endl;

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "Error while initializing freetype library." << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, (Configuration::getStringValue("ResourceBasePath") + file).c_str(), 0, &face))
        std::cout << "Failed to load font file " << file << std::endl;

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // load ASCII character set
    for (char c = 32; c < 127; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "Failed to load glyph " << c << std::endl;
                continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        Character character = {
                texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
        };
        m_characters.insert(std::make_pair(c, character));
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Font::free() {
    for (auto it = m_characters.begin(); it != m_characters.end(); it++)
        glDeleteTextures(1, &it->second.texture);
}

const Character& Font::getCharacter(char c) const {
    return m_characters.at(c);
}
