#pragma once

#include <glm/glm.hpp>

struct Rectangle {
    glm::vec2 position;
    glm::vec2 size;

    Rectangle(float x = 0, float y = 0, float width = 1, float height = 1);
    Rectangle(glm::vec2 position, glm::vec2 size);
};

