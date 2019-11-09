#pragma once

#include "pch.h"

#include "World.h"
#include "Camera.h"
#include "TextureAtlas.h"

#include "BlockShader.h"

class MasterRenderer {
private:
	BlockShader m_blockShader;

public:
	void init();
	
	void draw(const World &world, const Camera& camera);
};
