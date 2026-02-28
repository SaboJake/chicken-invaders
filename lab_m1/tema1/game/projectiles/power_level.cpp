#include "lab_m1/tema1/game/projectiles/power_level.h"

void PowerLevel::Move(float deltaTime) {
	y += speed * deltaTime;
}