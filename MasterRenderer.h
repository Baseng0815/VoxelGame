#pragma once

#include "pch.h"

#include "World.h"
#include "Camera.h"
#include "Shader.h"
#include "TextureAtlas.h"

class MasterRenderer {
private:
	Shader m_chunkShader;

public:
	void init();
	
	void draw(const World &world, const Camera& camera);
};
