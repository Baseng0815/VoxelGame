#include "pch.h"

#include "Definitions.h"

class GBuffer {
private:
	GLuint m_gBuffer;
	GLuint m_gPos, m_gNorm, m_gAlbedo;
	GLuint m_rboDepth;

public:
	int width = Definitions::WINDOW_WIDTH, height = Definitions::WINDOW_HEIGHT;

	void init();
	void resize(int width, int height);
	void bindFramebuffer(bool readOnly = false);
	void bindTextures();
};
