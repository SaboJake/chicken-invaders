#include "lab_m1/tema1/game/enemies/enemy.h"

void Enemy::Move(float deltaTime) {
	y -= speed * deltaTime;
}

bool Enemy::hitCheck(float hitX, float hitY) {
	return false;
}

glm::vec2 Enemy::getCenter() {
	return glm::vec2(x, y);
}

void Enemy::OffscreenCheck(float width, float height) {
	if (x < -200 || x > width + 200 || y < -200 || y > height + 200) {
		isActive = false;
	}
}