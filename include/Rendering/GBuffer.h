#pragma once

#include <GL/glew.h>

#include "../Configuration.h"

class GBuffer {
    private:
        GLuint m_gBuffer;
        GLuint m_gPos, m_gNorm, m_gAlbedo;
        GLuint m_rboDepth;

        void recreate();

        int m_width, m_height;

        void create(int width, int height);
        void destroy();

    public:
        GBuffer(int width, int height);
        ~GBuffer();

        int getWidth() const;
        int getHeight() const;
        void resize(int width, int height);
        void bindFramebuffer(bool readOnly = false);
        void bindTextures();
};
