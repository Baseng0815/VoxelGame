#pragma once

#include "Definitions.h"

typedef std::array<glm::vec2, 4> FaceUVs;

// texture atlas used for block rendering
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
	// texture coords for a single tile
	FaceUVs getTextureCoordinates(int index) const;

	// list of texture coordinates for a block type
	std::array<FaceUVs, 6> getBlockTextureCoordinates(BlockType blockType);

	void bind();
};
