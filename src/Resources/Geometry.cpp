#include "../../include/Resources/Geometry.h"

#include "../../include/Utility.h"

void Geometry::initBuffers() {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    // TODO disable unnecessary vertex attrib arrays when rendering other stuff
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(12));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(24));
    glBindVertexArray(0);
}

void Geometry::release()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

Geometry::Geometry(const std::string& file)
{
    // TODO implement .obj file loading
}

Geometry::Geometry(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    initBuffers();
    fillBuffers(vertices, indices);
}

Geometry::Geometry()
{
    initBuffers();
}

Geometry::~Geometry()
{
    release();
}

Geometry::Geometry(Geometry &&other) noexcept
    : m_vao(other.m_vao), m_vbo(other.m_vbo), m_ebo(other.m_ebo)
{
    other.m_vao = 0;
    other.m_vbo = 0;
    other.m_ebo = 0;
    other.m_drawCount = 0;
}

Geometry &Geometry::operator=(Geometry &&other)
{
    if (this != &other) {
        release();
        std::swap(m_vao, other.m_vao);
        std::swap(m_vbo, other.m_vbo);
        std::swap(m_ebo, other.m_ebo);
        std::swap(m_drawCount, other.m_drawCount);
    }

    return *this;
}

void Geometry::fillBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);

    m_drawCount = indices.size();
}

GLuint Geometry::getVao() const
{
    return m_vao;
}

GLuint Geometry::getVbo() const
{
    return m_vbo;
}

GLuint Geometry::getEbo() const
{
    return m_ebo;
}

unsigned int Geometry::getDrawCount() const
{
    return m_drawCount;
}
