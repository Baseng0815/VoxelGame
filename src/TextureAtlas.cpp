#include "../include/TextureAtlas.h"

#include <iostream>
#if defined(WIN32)
#include <SOIL.h>
#else
#include <SOIL/SOIL.h>
#endif

TextureAtlas::TextureAtlas(const char *fileName, int tileSize) {
    glGenTextures(1, &m_texture);

    int channels = 0;
    unsigned char* data = SOIL_load_image(fileName, &m_width, &m_height, &channels, SOIL_LOAD_RGBA);
    std::cout << SOIL_last_result() << std::endl;
    if (!data)
        throw std::runtime_error("Failed to load texture atlas.");

    m_numCols = m_width / tileSize;
    m_numRows = m_height / tileSize;
    m_uvXpT = 1 / (float)m_numCols;
    m_uvYpT = 1 / (float)m_numRows;

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    SOIL_free_image_data(data);

    FaceUVs faceUVs;
    // generate face uvs
    for (int i = 0; i < m_numCols * m_numRows; i++) {
        glm::vec2 topLeft = glm::vec2(i % m_numCols / (float)m_numCols , i / m_numCols / (float)m_numRows);
        m_faceUVs.push_back({ topLeft, glm::vec2(topLeft.x + m_uvXpT, topLeft.y + m_uvYpT),
                glm::vec2(topLeft.x + m_uvXpT, topLeft.y), glm::vec2(topLeft.x, topLeft.y + m_uvYpT)});
    }

    BlockUVs blockUVs;
    // generate block uv arrays
    for (int i = 0; i < m_blockUVsArray.size(); i++) {
        blockUVs[0] = m_faceUVs[(Definitions::BLOCK_DATA.at(i).tid_up)];
        blockUVs[1] = m_faceUVs[(Definitions::BLOCK_DATA.at(i).tid_nx)];
        blockUVs[2] = m_faceUVs[(Definitions::BLOCK_DATA.at(i).tid_px)];
        blockUVs[3] = m_faceUVs[(Definitions::BLOCK_DATA.at(i).tid_nz)];
        blockUVs[4] = m_faceUVs[(Definitions::BLOCK_DATA.at(i).tid_pz)];
        blockUVs[5] = m_faceUVs[(Definitions::BLOCK_DATA.at(i).tid_bot)];
        m_blockUVsArray[i] = blockUVs;
    }
}

const BlockUVsArray* TextureAtlas::getBlockTextureCoordinates() {
    return &m_blockUVsArray;
}

TextureAtlas::~TextureAtlas() {
    glDeleteTextures(1, &m_texture);
}

void TextureAtlas::bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}
