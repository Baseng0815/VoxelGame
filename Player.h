#pragma once

#include "Camera.h"

class Chunk;

enum class PlayMode : unsigned char {
	MODE_CREATIVE, MODE_SPECTATOR, MODE_SURVIVAL
};

class Player {
private:
	Camera m_camera;
	glm::vec3 m_position, m_front;

	void updateCamera();

public:
	Player();

	void update();
	void resolveCollisions(const Chunk* chunk);

	const Camera& getCamera() const;
};
