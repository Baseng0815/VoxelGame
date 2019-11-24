#pragma once

#include "pch.h"

struct Character {
	GLuint texture; // GL texture handle
	glm::ivec2 size; // size of glyph
	glm::ivec2 bearing; // offset from baseline to left/top
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
