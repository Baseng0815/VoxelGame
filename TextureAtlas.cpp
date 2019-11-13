#include "pch.h"

#include "soil.h"
#include "TextureAtlas.h"

void TextureAtlas::init(const char *fileName, int tileSize) {
	glGenTextures(1, &m_texture);

	int channels = 0;
	unsigned char* data = SOIL_load_image(fileName, &m_width, &m_height, &channels, SOIL_LOAD_RGBA);
	std::cout << SOIL_last_result() << std::endl;
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
	glm::vec2 topLeft = glm::vec2(index % m_numCols / (float)m_numCols , index / m_numCols / (float)m_numRows);
	return std::array<glm::vec2, 4> { topLeft, glm::vec2(topLeft.x + m_uvXpT, topLeft.y + m_uvYpT),
		glm::vec2(topLeft.x + m_uvXpT, topLeft.y), glm::vec2(topLeft.x, topLeft.y + m_uvYpT) };
}

std::array<FaceUVs, 6> TextureAtlas::getBlockTextureCoordinates(BlockType blockType) {
	std::array<FaceUVs, 6> output;
	output[0] = getTextureCoordinates(Definitions::BLOCK_DATA.at((int)blockType).tid_up);
	output[1] = getTextureCoordinates(Definitions::BLOCK_DATA.at((int)blockType).tid_nx);
	output[2] = getTextureCoordinates(Definitions::BLOCK_DATA.at((int)blockType).tid_px);
	output[3] = getTextureCoordinates(Definitions::BLOCK_DATA.at((int)blockType).tid_nz);
	output[4] = getTextureCoordinates(Definitions::BLOCK_DATA.at((int)blockType).tid_pz);
	output[5] = getTextureCoordinates(Definitions::BLOCK_DATA.at((int)blockType).tid_bot);
	return output;
}

TextureAtlas::~TextureAtlas() {
	glDeleteTextures(1, &m_texture);
}

void TextureAtlas::bind() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}