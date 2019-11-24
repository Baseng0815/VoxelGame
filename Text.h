#pragma once

#include "Colors.h"

struct Text {
	std::string text;
	glm::vec2 position = glm::vec2(0, 0);
	Color color = Colors::White;
	float scale = 1;
};
