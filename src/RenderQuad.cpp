#include "../include/RenderQuad.h"

#include "../include/Utility.h"

RenderQuad::RenderQuad(const Rectangle& area) {
    // TODO maybe optimize out into function
    float x1 = area.position.x * 2 - 1;
    float x2 = x1 + area.size.x * 2;
    float y1 = area.position.y * 2 - 1;
    float y2 = y1 + area.size.y * 2;

    float quadVertices[] = {
        // positions	    // texture Coords
        x1, y2, 0.0f, 1.0f,
        x1, y1, 0.0f, 0.0f,
        x2, y2, 1.0f, 1.0f,
        x2, y1, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, BUFFER_OFFSET(0));
}

RenderQuad::~RenderQuad() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void RenderQuad::render() const {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RenderQuad::resize(const Rectangle& area) {
    float x1 = area.position.x * 2 - 1;
    float x2 = x1 + area.size.x * 2;
    float y1 = area.position.y * 2 - 1;
    float y2 = y1 + area.size.y * 2;

    float quadVertices[] = {
        // positions	    // texture Coords
        x1, y2, 0.0f, 1.0f,
        x1, y1, 0.0f, 0.0f,
        x2, y2, 1.0f, 1.0f,
        x2, y1, 1.0f, 0.0f,
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
}
