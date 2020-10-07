#pragma once

#include <GL/glew.h>

enum class RenderbufferId {
    RENDERBUFFER_DEFAULT,
    RENDERBUFFER_REFRACTION,
    RENDERBUFFER_REFLECTION
};

class WaterRenderbuffers {
  private:
    GLuint m_refractionBuffer;
    GLuint m_reflectionBuffer;

    GLuint m_refractionDepthBuffer;
    GLuint m_reflectionDepthBuffer;

    GLuint m_refractionTexture;
    GLuint m_reflectionTexture;

    void destroy();
    void create(int width, int height);

  public:
    WaterRenderbuffers(int width, int height);
    ~WaterRenderbuffers();

    void bind(const RenderbufferId &id) const;

    void bindTextures(unsigned int offset = 0) const;

    void clear() const;

    void resize(int width, int height);
};