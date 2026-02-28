#include "lab_m1/tema1/game/projectiles/bullet.h"

void Bullet::Move(float deltaTime) {
	y += speed * deltaTime;
}

void Bullet::Despawn(int screenWidth, int screenHeight) {
	if (y - 500 > screenHeight || y < -500 || x < -500 || x - 500 > screenWidth) {
		isActive = false;
	}
}

glm::vec2 Bullet::getCenter() {
	return glm::vec2(x, y);
}