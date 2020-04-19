#pragma once

#include <vector>
#include <glm/glm.hpp>

struct CaveInfo {
	std::vector<glm::vec3> curve;
	glm::vec2 startChunk;
	glm::vec2 chunkPos;
	int position;	
	float lastRadius;	
};
