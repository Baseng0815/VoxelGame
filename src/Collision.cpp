#include "../include/Collision.h"

BoxCollision::BoxCollision(glm::vec3 pos, float width, float height, float length) : relativePos(pos), width(width), height(height), length(length) {}

std::vector<glm::vec3> BoxCollision::getAffectedBlocks(glm::vec3 position = glm::vec3()) const {
	std::vector<glm::vec3> positions = std::vector<glm::vec3>();

	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			for (int z = 0; z < length; z++) {
				positions.push_back(glm::floor(relativePos + position + glm::vec3(x, y, z)));
			}

	return positions;
}

glm::vec3 BoxCollision::getCenter(glm::vec3 position = glm::vec3())const {
	return relativePos + position + 0.5f * glm::vec3(width, height, length);
}

void BoxCollision::getMinMax(const glm::vec3 position, glm::vec3* min, glm::vec3* max) const {
	glm::vec3 pos1 = relativePos + position;
	glm::vec3 pos2 = relativePos + position + glm::vec3(width, height, length);

	if (pos1.x < pos2.x) {
		min->x = pos1.x;
		max->x = pos2.x;
	}
	else {
		min->x = pos2.x;
		max->x = pos1.x;
	}

	if (pos1.y < pos2.y) {
		min->y = pos1.y;
		max->y = pos2.y;
	}
	else {
		min->y = pos2.y;
		max->y = pos1.y;
	}

	if (pos1.z < pos2.z) {
		min->z = pos1.z;
		max->z = pos2.z;
	}
	else {
		min->z = pos2.z;
		max->z = pos1.z;
	}
}

#pragma region IntersectionInfo

IntersectionInfo::IntersectionInfo(const BoxCollision* colA, const BoxCollision* colB, const glm::vec3 posA, const glm::vec3 posB)
	: collisionA(colA), collisionB(colB), positionA(posA), positionB(posB) { 
	collisionA->getMinMax(posA, &aMin, &aMax);	
	collisionB->getMinMax(posB, &bMin, &bMax);

	intersects =
		aMin.x <= bMax.x && aMax.x >= bMin.x &&
		aMin.y <= bMax.y && aMax.y >= bMin.y &&
		aMin.z <= bMax.z && aMax.z >= bMin.z;
}

glm::vec3 IntersectionInfo::getMinimumTranslationVector() const {
	glm::vec3 possibleVectors[6];

	// positive x
	possibleVectors[0] = glm::vec3(bMax.x - aMin.x, 0, 0);
	// negative x
	possibleVectors[1] = glm::vec3(bMin.x - aMax.x, 0, 0);
	// positive y
	possibleVectors[2] = glm::vec3(0, bMax.y - aMin.y, 0);
	// negative y
	possibleVectors[3] = glm::vec3(0, bMin.y - aMax.y, 0);
	// positive z
	possibleVectors[4] = glm::vec3(0, 0, bMax.z - aMin.z);
	// negative z
	possibleVectors[5] = glm::vec3(0, 0, bMin.z - aMax.z);

	glm::vec3 mtv = glm::vec3();
	float minLength = FLT_MAX;

	for (int i = 0; i < 6; i++) {
		float length = glm::length(possibleVectors[i]);

		if (length < minLength && length != 0) {
			minLength = length;
			mtv = possibleVectors[i];
		}
	}

	return mtv;
}

#pragma endregion