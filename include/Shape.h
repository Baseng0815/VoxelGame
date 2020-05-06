#pragma once
#include <vector>
#include "../include/Vertex.h"

struct Triangle {
	glm::vec3 p1, p2, p3;
	glm::vec2 t1, t2, t3;
	glm::vec3 normal;
};

struct Shape {
private:
	void init();

public:
	std::vector<Triangle> triangles;
	float volume;
	glm::vec3 center;

	Shape(std::vector<Triangle>&);
};