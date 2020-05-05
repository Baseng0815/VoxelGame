#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// used by the master renderer to render the final texture onto
class RenderQuad {
    private:
        GLuint m_vao;
        GLuint m_vbo;

    public:
        RenderQuad();
        ~RenderQuad();

        void render();
};
