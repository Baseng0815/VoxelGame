#pragma once

#include <GL/glew.h>

struct BlockSelectionComponent {
    static float vertices[24];
    static unsigned int indices[24];

    GLuint m_vao, m_vbo, m_ebo;

    BlockSelectionComponent();

    void draw() const;
};
