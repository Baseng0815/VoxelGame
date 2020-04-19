#include "../include/CaveGenerator.h"
#include "../include/WorldGenerator.h"
#include "../include/Definitions.h"
#include "../include/Chunk.h"

std::vector<glm::vec3> CaveGenerator::getGenerationPoints(glm::vec3 start) const {
	std::vector<glm::vec3> points = std::vector<glm::vec3>();

	points.push_back(start);

	glm::vec3 dir = glm::normalize(glm::vec3(
		((float)rand() / RAND_MAX) * 2 - 1,
		((float)rand() / RAND_MAX) * 2 - 1,
		((float)rand() / RAND_MAX) * 2 - 1
	));

	// create new points
	for (int i = 1; i < length; i++) {
		float segmentLength = (maxSegmentLength - minSegmentLength) * (float)rand() / RAND_MAX + minSegmentLength;

		glm::vec3 newPoint = points[i - 1] + segmentLength * dir;
		points.push_back(newPoint);

		if (i != length - 1) {
			// create next direction vector
			float dx = (float)rand() / RAND_MAX * 1.6 - 0.8;
			float dy = (float)rand() / RAND_MAX * 1.6 - 0.8;
			float dz = (float)rand() / RAND_MAX * 1.6 - 0.8;

			dir = glm::normalize(glm::vec3(dir.x * dx, dir.y * dy, dir.z * dz));
		}

	}

	return points;
}

std::vector<glm::vec3> CaveGenerator::generateCaveCurve(glm::vec3 start) const {
	std::vector<glm::vec3> generationPoints = getGenerationPoints(start);

	std::vector<glm::vec3> curve = std::vector<glm::vec3>();

	float delta = (float)rand() / RAND_MAX * (float)(maxSegmentLength - minSegmentLength) / maxSegmentLength;
	//float delta = 0.01f;

	for (float d = 0; d < 1; d += delta) {
		glm::vec3 point = getBezierPoint(d, generationPoints);

		curve.push_back(point);
	}

	return curve;
}

glm::vec3 CaveGenerator::getBezierPoint(float t, std::vector<glm::vec3> points) const {
	int length = points.size();
	if (length == 1)
		return points[0];

	std::vector<glm::vec3> newPoints = std::vector<glm::vec3>();

	for (int p = 0; p < length - 1; p++) {
		glm::vec3 p1 = points[p];
		glm::vec3 p2 = points[p + 1];

		newPoints.push_back(p1 + t * (p2 - p1));
	}

	return getBezierPoint(t, newPoints);
}

void CaveGenerator::generateNewCave(const glm::vec2 position, Block*** blocks) {
	glm::vec3 start = glm::vec3(
		rand() % 16,	// x
		rand() % 64,	// y
		rand() % 16		// z
	);

	std::vector<glm::vec3> curve = generateCaveCurve(start);
	int curveSize = curve.size();
	int radius;
	for (int i = 0; i < curveSize - 1; i++) {
		glm::vec3 pos = curve[i];

		// point in chunk
		if (pos.x < Definitions::CHUNK_SIZE && pos.x >= 0 && pos.z < Definitions::CHUNK_SIZE && pos.z >= 0) {
			// fill sphere with air
			radius = (float)rand() / RAND_MAX * (maxCaveRadius - minCaveRadius) + minCaveRadius;
			int radiusSquare = pow(radius, 2);
			glm::vec3 delta = glm::vec3();

			for (delta.x = -radius; delta.x <= radius; delta.x++) {
				for (delta.z = -radius; delta.z <= radius; delta.z++) {
					// point in sphere
					if (pow(delta.x, 2) + pow(delta.z, 2) <= radiusSquare) {
						// calc height
						int height = (int)sqrt(radiusSquare - pow(delta.x, 2) - pow(delta.z, 2));

						for (delta.y = -height; delta.y <= height; delta.y++) {
							glm::vec3 newPos = pos + delta;
							if (Chunk::positionInRange(newPos)) {
								blocks[(int)newPos.x][(int)newPos.y][(int)newPos.z] = Block(BlockType::BLOCK_AIR);
							}
						}
					}
				}
			}
		}
		else {
			// create and store cave info
			CaveInfo info = CaveInfo();
			info.curve = curve;
			info.startChunk = position;
			info.lastRadius = radius;
			info.position = i - 1;

			generationInfo.push_back(std::make_pair(position, info));

			// continue till cave reaches this chunk
			while (!(pos.x < Definitions::CHUNK_SIZE && pos.x >= 0 && pos.z < Definitions::CHUNK_SIZE && pos.z >= 0) && ++i < curveSize - 1) {
				pos = curve[i];
			}
		}
	}
}

void CaveGenerator::continueGeneration(CaveInfo& info, const glm::vec2 position, Block*** blocks) {
	glm::vec3 offset = glm::vec3(
		info.startChunk.x - position.x,
		0,
		info.startChunk.y - position.y
	);

	int curveSize = info.curve.size();
	int radius = info.lastRadius;
	for (int i = info.position; i < curveSize; i++) {
		glm::vec3 pos = info.curve[i] + offset;

		// point in chunk
		if (pos.x < Definitions::CHUNK_SIZE && pos.x >= 0 && pos.z < Definitions::CHUNK_SIZE && pos.z >= 0) {
			// fill sphere with air
			radius = (float)rand() / RAND_MAX * (maxCaveRadius - minCaveRadius) + minCaveRadius;
			int radiusSquare = pow(radius, 2);
			glm::vec3 delta = glm::vec3();

			for (delta.x = -radius; delta.x <= radius; delta.x++) {
				for (delta.z = -radius; delta.z <= radius; delta.z++) {
					// point in sphere
					if (pow(delta.x, 2) + pow(delta.z, 2) <= radiusSquare) {
						// calc height
						int height = (int)sqrt(radiusSquare - pow(delta.x, 2) - pow(delta.z, 2));

						for (delta.y = -height; delta.y <= height; delta.y++) {
							glm::vec3 newPos = pos + delta;
							if (Chunk::positionInRange(newPos)) {
								blocks[(int)newPos.x][(int)newPos.y][(int)newPos.z] = Block(BlockType::BLOCK_AIR);
							}
						}
					}
				}
			}
		}
		else {
			// create and store cave info
			CaveInfo newInfo = CaveInfo();
			newInfo.curve = info.curve;
			newInfo.startChunk = info.startChunk;
			newInfo.lastRadius = radius;
			newInfo.position = i - 1;

			generationInfo.push_back(std::make_pair(position, newInfo));

			// continue till cave reaches this chunk
			while (!(pos.x < Definitions::CHUNK_SIZE && pos.x >= 0 && pos.z < Definitions::CHUNK_SIZE && pos.z >= 0) && ++i < curveSize - 1) {
				pos = info.curve[i] + offset;
			}
		}
	}
}

void CaveGenerator::generate(const glm::vec2 position, Block*** blocks, int cavesCount) {
	// generate new caves
	for (int i = 0; i < cavesCount; i++) {
		generateNewCave(position, blocks);
	}

	// Does not work yet because chunk generation is async
	// complete generation of cave begins	
	//glm::vec2 right = glm::vec2(1, 0);
	//glm::vec2 back = glm::vec2(0, -1);

	//for (int i = 0; i < generationInfo.size(); i++) {
	//	glm::vec2 chunkPos = generationInfo[i].first;

	//	// right chunk
	//	if (chunkPos == position + right
	//		// back
	//		|| chunkPos == position + back
	//		// left
	//		|| chunkPos == position - right
	//		// front
	//		|| chunkPos == position - back) {

	//		auto pair = generationInfo.begin() + i;
	//		continueGeneration((*pair).second, position, blocks);

	//		generationInfo.emplace(pair);
	//	}
	//}
}