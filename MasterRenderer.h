#pragma once

#include "pch.h"

#include "World.h"
#include "Camera.h"
#include "GBuffer.h"
#include "RenderQuad.h"
#include "TextureAtlas.h"

#include "BlockShader.h"
#include "LightingShader.h"

class MasterRenderer {
private:
	// first render pass
	BlockShader m_blockShader;
	LightingShader m_lightingShader;
	
	// second render pass
	Light m_light;
	GBuffer m_gBuffer;
	RenderQuad m_renderQuad;

public:
	void init();
	void resize(int width, int height);
	void render(const World &world, const Camera& camera);
};
