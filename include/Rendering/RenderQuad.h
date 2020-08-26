#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Gui/Rectangle.h"

// used for rendering 2d textures directly on-screen
// TODO add shape system and incorporate it into resource manager
class RenderQuad {
    private:
        GLuint m_vao;
        GLuint m_ebo;
        GLuint m_vbo;

    public:
        // area in screen space
        RenderQuad();
        RenderQuad(const Rectangle &area, bool flipY = false);
        ~RenderQuad();

        void render() const;
        void resize(const Rectangle &area, bool flipY = false) const;
};
