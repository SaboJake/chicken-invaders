#include "lab_m1/tema1/game/projectiles/egg.h"

void Egg::Move(float deltaTime) {
	y += speed * deltaTime;
}