#pragma once

#include "pch.h"

class CubeMesh {
public:

	// define six vertices for each side, not using indices
	std::array<glm::vec3, 6> PX_POS = {
		glm::vec3(0.5f,0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,0.5f),
		glm::vec3(0.5f,-0.5f,0.5f),
		glm::vec3(0.5f,0.5f,0.5f),
		glm::vec3(0.5f,0.5f,-0.5f)
	};
	
	std::array<glm::vec3, 6> NX_POS = {
		glm::vec3(-0.5f,0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,0.5f),
		glm::vec3(-0.5f,-0.5f,0.5f),
		glm::vec3(-0.5f,0.5f,0.5f),
		glm::vec3(-0.5f,0.5f,-0.5f)
	};
	
	std::array<glm::vec3, 6> PY_POS = {
		glm::vec3(-0.5f,0.5f,0.5f),
		glm::vec3(-0.5f,0.5f,-0.5f),
		glm::vec3(0.5f,0.5f,-0.5f),
		glm::vec3(0.5f,0.5f,-0.5f),
		glm::vec3(0.5f,0.5f,0.5f),
		glm::vec3(-0.5f,0.5f,0.5f)
	};
	
	std::array<glm::vec3, 6> NY_POS = {
		glm::vec3(-0.5f,-0.5f,0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,0.5f),
		glm::vec3(-0.5f,-0.5f,0.5f)
	};
	
	std::array<glm::vec3, 6> PZ_POS = {
		glm::vec3(-0.5f,0.5f,0.5f),
		glm::vec3(-0.5f,-0.5f,0.5f),
		glm::vec3(0.5f,-0.5f,0.5f),
		glm::vec3(0.5f,-0.5f,0.5f),
		glm::vec3(0.5f,0.5f,0.5f),
		glm::vec3(-0.5f,0.5f,0.5f)
	};
	
	std::array<glm::vec3, 6> NZ_POS = {
		glm::vec3(-0.5f,0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f,0.5f,-0.5f),
		glm::vec3(-0.5f,0.5f,-0.5f)
	};

	std::array<glm::vec2, 6> UV = {
		glm::vec2(0.f, 0.f),
		glm::vec2(0.f, 1.f),
		glm::vec2(1.f, 1.f),
		glm::vec2(1.f, 1.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(0.f, 0.f)
	};
	
	std::array<glm::vec3, 6> NORMALS = {
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 0.f)
	};
}
