#pragma once

#include "pch.h"

#include "World.h"
#include "Skybox.h"
#include "Camera.h"
#include "GBuffer.h"
#include "RenderQuad.h"
#include "TextureAtlas.h"

#include "GUIRenderer.h"

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
	SkyboxShader m_skyboxShader;
	Skybox m_skybox;

	TextureAtlas m_textureAtlas;

	GUIRenderer m_guiRenderer;
	
	// @TODO
	// add ptr to GUI handler, abstract GUI elements (ECS)
	std::vector<Text*> m_texts;

public:
	void init();

	void addText(Text*);
	void removeText(Text*);

	void resize(int width, int height);
	void render(const World &world, const Camera& camera);
};
