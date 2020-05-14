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

struct Ray {
	glm::vec3 start, direction;

	Ray() = default;
	Ray(glm::vec3 start, glm::vec3 dir);

	std::vector<glm::ivec3> getAffectedBlocks(float length) const;
};

typedef std::pair<glm::vec3, glm::vec3> Line;

class Collision {
public:
	static bool intersects(const BoxCollision* colA, const glm::vec3 posA,
		const BoxCollision* colB, const glm::vec3 posB);

	static std::vector<glm::vec3> getTranslationVectors(const BoxCollision* colA, const glm::vec3 posA,
		const BoxCollision* colB, const glm::vec3 posB);	
};