#include "pch.h"

#include "soil.h"
#include "TextureAtlas.h"

const std::map<BLOCK_TYPE, std::array<int, 6>> TextureAtlas::BLOCK_TEX_MAPPINGS = {
	{ BLOCK_DIRT, std::array<int, 6>{ 0, 0, 0, 0, 0, 0 }},
	{ BLOCK_COBBLESTONE, std::array<int, 6>{ 1, 1, 1, 1, 1, 1 }},
	{ BLOCK_GRASS, std::array<int, 6>{ 2, 3, 3, 3, 3, 0 }},
	{ BLOCK_STONE, std::array<int, 6>{ 4, 4, 4, 4, 4, 4 }}
};

void TextureAtlas::init(const char *fileName, int tileSize) {
	glGenTextures(1, &m_texture);

	int channels = 0;
	unsigned char* data = SOIL_load_image(fileName, &m_width, &m_height, &channels, SOIL_LOAD_AUTO);
	if (!data)
		throw std::runtime_error("Failed to load texture atlas.");

	m_numCols = m_width / tileSize;
	m_numRows = m_height / tileSize;
	m_uvXpT = 1 / (float)m_numCols;
	m_uvYpT = 1 / (float)m_numRows;

	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	SOIL_free_image_data(data);
}

FaceUVs TextureAtlas::getTextureCoordinates(int index) const {
	glm::vec2 topLeft = glm::vec2(index % m_numCols / (float)m_numCols , m_numRows - index / m_numCols / (float)m_numRows);
	return std::array<glm::vec2, 4> { topLeft, glm::vec2(topLeft.x + m_uvXpT, topLeft.y + m_uvYpT),
		glm::vec2(topLeft.x + m_uvXpT, topLeft.y), glm::vec2(topLeft.x, topLeft.y + m_uvYpT) };
}

std::array<FaceUVs, 6> TextureAtlas::getBlockTextureCoordinates(BLOCK_TYPE blockType) {
	std::array<FaceUVs, 6> output;
	for (int i = 0; i < 6; i++)
		output[i] = getTextureCoordinates(BLOCK_TEX_MAPPINGS.at(blockType)[i]);
	return output;
}

void TextureAtlas::cleanUp() {
	glDeleteTextures(1, &m_texture);
}

void TextureAtlas::bind() {
	glBindTexture(GL_TEXTURE_2D, m_texture);
}