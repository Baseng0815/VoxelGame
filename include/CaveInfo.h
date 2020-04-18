#pragma once

#include <vector>
#include <glm/glm.hpp>

struct CaveInfo {
	std::vector<glm::vec3> curve;
	int position;	
	float lastRadius;	
};
