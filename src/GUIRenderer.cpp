#include "../include/GUIRenderer.h"

void GUIRenderer::renderText(const Text* text) const {
	float px = text->position.x;
	m_guiShader.setTextColor(text->color);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_textVao);

	for (auto it = text->text.begin(); it != text->text.end(); it++) {
		const Character& ch = m_font.getCharacter(*it);

		GLfloat xpos = px + ch.bearing.x * text->scale;
		GLfloat ypos = text->position.y - (ch.size.y - ch.bearing.y) * text->scale;

		GLfloat w = ch.size.x * text->scale;
		GLfloat h = ch.size.y * text->scale;

		GLfloat vertices[6][4] = {
			{ xpos	  , ypos + h, 0, 0 },
			{ xpos	  , ypos	, 0, 1 },
			{ xpos + w, ypos	, 1, 1 },

			{ xpos	  , ypos + h, 0, 0 },
			{ xpos + w, ypos	, 1, 1 },
			{ xpos + w, ypos + h, 1, 0 }
		};

		glBindTexture(GL_TEXTURE_2D, ch.texture);
		glBindBuffer(GL_ARRAY_BUFFER, m_textVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		px += (ch.advance >> 6)* text->scale;
	}
}

void GUIRenderer::init() {
	m_font.init("Resources/Fonts/arial.ttf");
	m_guiShader.init();

	glGenVertexArrays(1, &m_textVao);
	glGenBuffers(1, &m_textVbo);
	glBindVertexArray(m_textVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_textVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
}

void GUIRenderer::render(const std::vector<Text*>& text) const {
	m_guiShader.bind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (const auto t : text) {
		renderText(t);
	}

	glDisable(GL_BLEND);
}
