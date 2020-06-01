#pragma once

#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../Resource.h"

struct Character {
    GLuint texture; // GL texture handle
    glm::ivec2 size; // size of glyph
    glm::ivec2 bearing; // offset from baseline to left/top
    GLuint advance; // offset to next glyph
};

class Font : public Resource {
    private:
        std::map<char, Character> m_characters;

    public:
        Font(const std::string& file);
        ~Font();

        const Character& getCharacter(char c) const;
};
