#pragma once

#include <glm/glm.hpp>

struct Rectangle {
    glm::vec2 position;
    glm::vec2 size;

    Rectangle(float x = 0.f, float y = 0.f, float width = 1.f, float height = 1.f);
    Rectangle(glm::vec2 position, glm::vec2 size);
};

