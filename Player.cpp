#include "pch.h"

#include "Player.h"

void Player::updateCamera() {
	m_front = m_camera.update(m_position);
}

Player::Player() {
	m_camera.han
}