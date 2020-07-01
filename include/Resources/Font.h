#pragma once

#include <map>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Resource.h"

struct Character {
    GLuint texture; // GL texture handle
    glm::vec2 size; // size of glyph
    glm::vec2 bearing; // offset from baseline to left/top
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
