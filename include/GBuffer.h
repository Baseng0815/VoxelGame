#pragma once

#include <GL/glew.h>

#include "Definitions.h"

class GBuffer {
    private:
        GLuint m_gBuffer;
        GLuint m_gPos, m_gNorm, m_gAlbedo;
        GLuint m_rboDepth;

        void recreate();

        int width = Definitions::WINDOW_WIDTH,
            height = Definitions::WINDOW_HEIGHT;

        void create();
        void destroy();

    public:

        GBuffer();
        ~GBuffer();

        int getWidth() const;
        int getHeight() const;
        void resize(int width, int height);
        void bindFramebuffer(bool readOnly = false);
        void bindTextures();
};
