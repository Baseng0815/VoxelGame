#include "pch.h"

#include "Skybox.h"
#include "soil.h"

void Skybox::init() {
	glGenTextures(1, &m_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	int width, height, channels;
	std::string path;
	for (int i = 0; i < 6; i++) {
		path = "Resources/Textures/Skybox/1_" + std::to_string(i) + ".png";
		unsigned char* data = SOIL_load_image(path.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
		if (!data)
			std::cout << "Failed to load skybox image data" << std::endl;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		SOIL_free_image_data(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Skybox::cleanUp() {
	glDeleteTextures(1, &m_texture);
}

void Skybox::bind() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}
