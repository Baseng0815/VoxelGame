#pragma once

#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Character {
    GLuint texture; // GL texture handle
    glm::vec2 size; // size of glyph
    glm::vec2 bearing; // offset from baseline to left/top
    GLuint advance; // offset to next glyph
};

class Font {
    private:
        std::map<char, Character> m_characters;

    public:
        void init(const char* file);
        ~Font();

        const Character& getCharacter(char c) const;
};
