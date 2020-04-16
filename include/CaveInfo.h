#pragma once
#include "pch.h"

struct CaveInfo {
	std::vector<glm::vec3> curve;
	int position;	
	float lastRadius;	
};