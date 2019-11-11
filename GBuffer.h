#include "pch.h"

class GBuffer {
private:
	GLuint m_gBuffer;
	GLuint m_gPos, m_gNorm, m_gAlbedo;
	GLuint m_rboDepth;

public:
	void init();
	void resize(int width, int height);
	void bindFramebuffer();
	void bindTextures();
};
