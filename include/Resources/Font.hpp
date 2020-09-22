#pragma once

#include <map>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Resource.hpp"

struct Character {
    GLuint texture; // GL texture handle
    glm::vec2 size; // size of glyph
    glm::vec2 bearing; // offset from baseline to left/top
    GLuint advance; // offset to next glyph
};

class Font : public Resource {
    private:
        std::map<char, Character> m_characters;

        void release() override;

    public:
        Font(const std::string& file);
        ~Font();

        Font(const Font&) = delete;
        Font &operator=(const Font&) = delete;
        Font(Font&&) noexcept;
        Font &operator=(Font&&) noexcept;

        const Character& getCharacter(char c) const;
};
