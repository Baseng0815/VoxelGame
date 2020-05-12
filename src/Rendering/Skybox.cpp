#include "../../include/Rendering/Skybox.h"
#include "../../include/Utility.h"

#include <string>
#include <iostream>
#if defined(WIN32)
#include <SOIL.h>
#else
#include <SOIL/SOIL.h>
#endif

void Skybox::init() {
    // textures
    glGenTextures(1, &m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

    int width, height, channels;
    std::string path;
    for (int i = 0; i < 6; i++) {
        path = "Resources/Textures/Skybox/1_" + std::to_string(i) + ".png";
        unsigned char* data = SOIL_load_image(path.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
        if (!data)
            std::cout << "Failed to load skybox image data" << std::endl;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        SOIL_free_image_data(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // geometry
    float SIZE = 6000;

    const float vertices[] = {
        -SIZE, SIZE, -SIZE,
        -SIZE, -SIZE, SIZE,
        -SIZE, SIZE, SIZE,
        -SIZE, -SIZE, -SIZE,
        SIZE, SIZE, SIZE,
        SIZE, -SIZE, -SIZE,
        SIZE, SIZE, -SIZE,
        SIZE, -SIZE, SIZE
    };

    const unsigned int indices[] = {
        0, 1, 2,
        0, 3, 1,
        0, 4, 6,
        0, 2, 4,
        4, 5, 6,
        4, 7, 5,
        2, 7, 4,
        2, 1, 7,
        6, 3, 0,
        6, 5, 3,
        1, 5, 7,
        1, 3, 5
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

Skybox::~Skybox() {
    glDeleteTextures(1, &m_texture);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

void Skybox::render() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

    glDisable(GL_CULL_FACE);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_CULL_FACE);
}
