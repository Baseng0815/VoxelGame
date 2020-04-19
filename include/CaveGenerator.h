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

	std::vector<std::pair<glm::vec2, CaveInfo>> generationInfo;
	void generateNewCave(const glm::vec2 position, Block*** blocks);
	void continueGeneration(CaveInfo& info, const glm::vec2 position, Block*** blocks);

public:			
	float maxSegmentLength = 50;
	float minSegmentLength = 3;

	int minCaveRadius = 2;
	int maxCaveRadius = 5;
	int minHeight = 0;
	int maxHeight = 64;

	int length = 5;

	void generate(const glm::vec2 position, Block*** blocks, int cavesCount);
};

