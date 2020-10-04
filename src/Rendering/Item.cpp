#include "../../include/Rendering/Item.hpp"

#include "../../include/GameData/BlockTemplate.hpp"
#include "../../include/GameData/GameData.hpp"
#include "../../include/Utility.hpp"

Item::Item() {
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

Item::Item(const Rectangle &area, const BlockId &block) {
    resize(area, block);
}

Item::~Item() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void Item::render() const {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, nullptr);
}

void Item::resize(const Rectangle &area, const BlockId &block) const {
    float x1 = area.position.x;
    float x2 = x1 + area.size.x / 2;
    float x3 = x1 + area.size.x;

    float y1 = area.position.y;
    float y2 = y1 + 0.25f * area.size.y;
    float y3 = y1 + 0.50f * area.size.y;
    float y4 = y1 + 0.75f * area.size.y;
    float y5 = y1 + area.size.y;

    BlockTemplate tmp = GameData::getBlockTemplate(block);
    float uvXpT = 1 / (float)16;
    float uvYpT = 1 / (float)16;

    float uvX1 = (tmp.tid_nx % 16) * uvXpT;
    float uvY1 = (tmp.tid_nx / 16) * uvYpT;

    float uvX2 = (tmp.tid_ny % 16) * uvXpT;
    float uvY2 = (tmp.tid_ny / 16) * uvYpT;

    float uvX3 = (tmp.tid_pz % 16) * uvXpT;
    float uvY3 = (tmp.tid_pz / 16) * uvYpT;

    // clang-format off
    float vertices[] = {
        x1, y2, uvX1,           uvY1 + uvYpT,
        x2, y1, uvX1 + uvXpT,   uvY1 + uvYpT,
        x2, y3, uvX1,           uvY1,
        x1, y4, uvX1 + uvXpT,   uvY1,

        x2, y1, uvX2,           uvY2 + uvYpT,
        x3, y2, uvX2 + uvXpT,   uvY2 + uvYpT,
        x3, y4, uvX2,           uvY2,
        x2, y3, uvX2 + uvXpT,   uvY2,         
         
        x2, y3, uvX3,           uvY3 + uvYpT,
        x3, y4, uvX3 + uvXpT,   uvY3 + uvYpT,
        x2, y5, uvX3,           uvY3,
        x1, y4, uvX3 + uvXpT,   uvY3,         
    };

    unsigned int indices[] = {
        0, 1, 2, 0, 2, 3, 
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11
    };

    // clang-format on

    glNamedBufferData(m_vbo, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glNamedBufferData(m_ebo, sizeof(indices), indices, GL_DYNAMIC_DRAW);
}