#include "../../include/Components/BlockSelectionComponent.hpp"

float BlockSelectionComponent::vertices[24] = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f};

unsigned int BlockSelectionComponent::indices[24] = {
    0, 1, 1, 2, 2, 3, 3, 0,
    4, 5, 5, 6, 6, 7, 7, 4,
    4, 0, 5, 1, 6, 2, 7, 3};

BlockSelectionComponent::BlockSelectionComponent() {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glNamedBufferData(m_vbo, 24 * sizeof(float), BlockSelectionComponent::vertices, GL_STATIC_DRAW);
    glNamedBufferData(m_ebo, 24 * sizeof(unsigned int), BlockSelectionComponent::indices, GL_STATIC_DRAW);
}

void BlockSelectionComponent::draw() const {
    glBindVertexArray(m_vao);
    // glEnable(GL_DEPTH_TEST);        

    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, nullptr);    
}
