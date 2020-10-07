#pragma once

#include <GL/glew.h>

enum class RenderbufferId {
    RENDERBUFFER_DEFAULT,
    RENDERBUFFER_SCENE,
    RENDERBUFFER_WATER
};

class WaterRenderbuffers {
  private:
    GLuint m_sceneBuffer;
    GLuint m_waterBuffer;

    GLuint m_sceneDepthBuffer;
    GLuint m_waterDepthBuffer;

    GLuint m_sceneTexture;
    GLuint m_waterTexture;

    void destroy();
    void create(int width, int height);

  public:
    WaterRenderbuffers(int width, int height);
    ~WaterRenderbuffers();

    void bind(const RenderbufferId &id) const;

    void bindTextures() const;

    void clear() const;

    void resize(int width, int height);
};