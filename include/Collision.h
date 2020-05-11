#pragma once
#include <glm/glm.hpp>
#include <vector>

struct BoxCollision {
	glm::vec3 relativePos;	
	bool dynamic = true;
	float width = 1, height = 1, length = 1;

	BoxCollision() = default;
	BoxCollision(glm::vec3, float, float, float);	

	std::vector<glm::vec3> getAffectedBlocks(glm::vec3) const;
	glm::vec3 getCenter(glm::vec3) const;	

	void getMinMax(const glm::vec3 position, glm::vec3* min, glm::vec3* max) const;
};

struct IntersectionInfo {
private:
	glm::vec3 aMin; glm::vec3 aMax; glm::vec3 bMin; glm::vec3 bMax;
public:
	const BoxCollision* collisionA;
	const BoxCollision* collisionB;

	const glm::vec3 positionA;
	const glm::vec3 positionB;	

	bool intersects;

	IntersectionInfo(const BoxCollision*, const BoxCollision*, const glm::vec3, const glm::vec3);	

	glm::vec3 getMinimumTranslationVector() const;
};