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

BoxCollision BoxCollision::applyTransformation(const TransformationComponent& transform) const {
	BoxCollision transBox = BoxCollision();

	glm::mat4 modelMat = transform.getModelMatrix();
	glm::mat4 rotationMatrix =
		glm::rotate(transform.rotation.z, glm::vec3(0, 0, 1)) *
		glm::rotate(transform.rotation.y, glm::vec3(0, 1, 0)) *
		glm::rotate(transform.rotation.x, glm::vec3(1, 0, 0));


	transBox.pos = modelMat * glm::vec4(pos, 1.0f);
	transBox.a = rotationMatrix * glm::vec4(a, 1.0f);
	transBox.b = rotationMatrix * glm::vec4(b, 1.0f);
	transBox.c = rotationMatrix * glm::vec4(c, 1.0f);

		return transBox;
}

std::vector<glm::vec3> BoxCollision::getAffectedBlocks() const {
	std::vector<glm::vec3> positions = std::vector<glm::vec3>();

	glm::vec3 dirA = glm::normalize(a);
	glm::vec3 dirB = glm::normalize(b);
	glm::vec3 dirC = glm::normalize(c);

	float lengthA = length(a);
	float lengthB = length(b);
	float lengthC = length(c);

	glm::vec3 lastPos = floor(pos);
	positions.push_back(lastPos);

	for (glm::vec3 pA = glm::vec3(); length(pA) <= lengthA; pA += dirA)
		for (glm::vec3 pB = glm::vec3(); length(pB) <= lengthB; pB += dirB)
			for (glm::vec3 pC = glm::vec3(); length(pC) <= lengthC; pC += dirC) {
				glm::vec3 p = glm::round(pos + pA + pB + pC);

				if (std::find(positions.begin(), positions.end(), p) == positions.end())  {
					positions.push_back(p);
				}
			}


	return positions;
}