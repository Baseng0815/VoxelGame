#pragma once

#include "pch.h"

#include "World.h"
#include "Skybox.h"
#include "Camera.h"
#include "GBuffer.h"
#include "RenderQuad.h"
#include "TextureAtlas.h"
#include "RenderSkybox.h"

#include "BlockShader.h"
#include "SkyboxShader.h"
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

	// skybox
	RenderSkybox m_renderSkybox;
	SkyboxShader m_skyboxShader;
	Skybox m_skybox;

public:
	void init();
	void cleanUp();

	void resize(int width, int height);
	void render(const World &world, const Camera& camera);
};
