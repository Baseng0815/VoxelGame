#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Rectangle.h"

// used for rendering 2d textures directly on-screen
// TODO add shape system and incorporate it into resource manager
class RenderQuad {
    private:
        GLuint m_vao;
        GLuint m_vbo;

        float x1, y1, x2, y2;

    public:
        // area in 0,0-1,1 screen range
        RenderQuad(const Rectangle& area = Rectangle());
        ~RenderQuad();

        void render() const;
        void resize(const Rectangle& area);
};
