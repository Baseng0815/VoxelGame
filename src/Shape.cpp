#include "../include/Shape.h"

#include "../include/Components/TransformationComponent.h"

Shape::Shape(std::vector<Triangle>& triangles) {
	this->triangles = std::vector<Triangle>(triangles);

	init();
}

void Shape::init() {
	volume = 0;
	glm::vec3 centerSum = glm::vec3();

	for (auto tri : triangles) {
		//		1		->  ->    ->
		//	V = -	* | (a x b) * c |
		//		6
		float v = 1 / (float)6 * abs(glm::dot((glm::cross(tri.p1, tri.p2 - tri.p1)), tri.p3 - tri.p1));

		// subtract volume if normal points to origin
		if (glm::dot(tri.p1, tri.normal) < 0) {
			v *= -1;
		}
		volume += v;

		// center calculation
		glm::vec3 triCenter = (tri.p1 + tri.p2 + tri.p3) / 3.0f;

		centerSum += triCenter;
	}

	center = centerSum * (1.0f / triangles.size());
}
