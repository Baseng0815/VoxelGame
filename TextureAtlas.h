#pragma once

#include "pch.h"

typedef std::array<glm::vec2, 4> FaceUVs;

class TextureAtlas {
private:
	GLuint m_texture;
	int m_width, m_height;
	int m_numRows, m_numCols;

	// uv per tile
	float m_uvXpT, m_uvYpT;

public:
	void init(const char* fileName, int tileSize = 16);
	void cleanUp();

	// top-left, bottom-right, top-right, bottom-left
	FaceUVs getTextureCoordinates(int index) const;

	void bind();
};
