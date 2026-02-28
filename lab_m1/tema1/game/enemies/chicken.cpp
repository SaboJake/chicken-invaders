#include "lab_m1/tema1/game/enemies/chicken.h"
#include <iostream>

int moveDir = 1;

void Chicken::Move(float deltaTime) {
	if (movePattern) {
		movePattern(*this, deltaTime);
	}
}

bool Chicken::hitCheck(float hitX, float hitY) {
	if (!isActive) return false;
	float dx = hitX - getCenter().x;
	float dy = hitY - getCenter().y;

	return (dx * dx) / (hitboxR.x * hitboxR.x) +
		(dy * dy) / (hitboxR.y * hitboxR.y) <= 1.0f;
}

glm::vec2 Chicken::getCenter() {
	return glm::vec2(x, y + 64);
}

void Chicken::ResetEggTimer() {
	float rnd = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	timeEgg = 2.0f + rnd * 3.0f;
}

bool Chicken::SpawnEgg(float deltaTime) {
	timeEgg -= deltaTime;
	if (timeEgg <= 0.0f) {
		ResetEggTimer();
		return true;
	}
	return false;
}