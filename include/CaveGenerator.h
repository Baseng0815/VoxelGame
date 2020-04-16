#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "CaveInfo.h"
#include "Block.h"

class WorldGenerator;

class CaveGenerator {
private:
	std::vector<glm::vec3> getGenerationPoints(glm::vec3 start) const;
	std::vector<glm::vec3> generateCaveCurve(glm::vec3 start) const;
	glm::vec3 getBezierPoint(float t, std::vector<glm::vec3> points) const;

	WorldGenerator* worldGenerator;

public:			
	float maxSegmentLength = 15;
	float minSegmentLength = 5;

	int minCaveRadius = 2;
	int maxCaveRadius = 5;
	int minHeight = 0;
	int maxHeight = 64;

	int length;

	void generateCave(const glm::vec2 position, Block*** blocks) const;
};

