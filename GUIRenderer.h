#pragma once

#include "Font.h"
#include "Text.h"
#include "GUIShader.h"

class GUIRenderer {
private:
	Font m_font;
	GUIShader m_guiShader;

	GLuint m_textVao, m_textVbo;
	void renderText(const Text* text) const;

public:
	void init();

	void render(const std::vector<Text*>& text) const;
};
