#include "../../include/Rendering/RenderQuad.h"

#include "../../include/Utility.h"

RenderQuad::RenderQuad()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // create empty buffers
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, BUFFER_OFFSET(0));
}

RenderQuad::RenderQuad(const Rectangle& area, bool flipY)
    : RenderQuad()
{
    resize(area, flipY);
}

RenderQuad::~RenderQuad()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void RenderQuad::render() const
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RenderQuad::resize(const Rectangle& area, bool flipY) const
{
    float x1 = area.position.x;
    float x2 = x1 + area.size.x;
    float y1 = area.position.y;
    float y2 = y1 + area.size.y;

    float quadVertices[] = {
        // positions	    // texture Coords
        x1, y1, 0.0f, flipY ? 0.0f : 1.0f,
        x2, y2, 1.0f, flipY ? 1.0f : 0.0f,
        x2, y1, 1.0f, flipY ? 0.0f : 1.0f,
        x1, y2, 0.0f, flipY ? 1.0f : 0.0f,
    };

    unsigned int indices[]= {
        0, 2, 1, 0, 1, 3
    };

    glNamedBufferData(m_vbo, sizeof(quadVertices), quadVertices, GL_DYNAMIC_DRAW);
    glNamedBufferData(m_ebo, sizeof(indices), indices, GL_DYNAMIC_DRAW);
}
