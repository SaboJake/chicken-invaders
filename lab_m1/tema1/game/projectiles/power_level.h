#pragma once

#include "lab_m1/tema1/game/projectiles/bullet.h"

class PowerLevel : public Bullet {
	public:
	PowerLevel(Mesh* mesh, float x, float y, float z, float speed, int targetType, int damage)
		: Bullet(mesh, x, y, z, speed, targetType, damage) {
		type = 1000; // Power Level type
	}
	Bullet* Clone() const override {
		return new PowerLevel(*this);
	}
	
	void Move(float deltaTime) override;
};