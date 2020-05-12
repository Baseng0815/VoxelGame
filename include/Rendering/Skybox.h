#pragma once

#include <GL/glew.h>

class Skybox {
private:
    GLuint m_texture;
    GLuint m_vao;
    GLuint m_vbo, m_ebo;

public:
    void init();
    ~Skybox();

    void render();
};
