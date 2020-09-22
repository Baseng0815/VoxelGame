#include "../../include/Gui/Rectangle.hpp"

Rectangle::Rectangle(float x, float y, float width, float height)
    : position(x, y), size(width, height) {}

Rectangle::Rectangle(glm::vec2 position, glm::vec2 size)
    : position(position), size(size) {}

