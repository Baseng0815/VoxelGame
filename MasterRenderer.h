#pragma once

#include "pch.h"

#include "Chunk.h"
#include "Camera.h"
#include "Shader.h"

class MasterRenderer {
private:
	Shader m_chunkShader;

public:
	void init();
	
	void draw(const std::vector<Chunk>& chunks, const Camera& camera);

};
