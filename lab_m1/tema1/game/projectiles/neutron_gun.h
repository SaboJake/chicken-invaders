#pragma once

#include "lab_m1/tema1/game/projectiles/bullet.h"

class NeutronGun : public Bullet {
public:
	NeutronGun(Mesh* mesh, float x, float y, float z, float speed, int targetType, int damage)
		: Bullet(mesh, x, y, z, speed, targetType, damage) {
		type = 1; // Neutron Gun type
	}

	NeutronGun(Mesh* mesh, glm::mat4 offset, float x, float y, float z, float speed, int targetType, int damage)
		: Bullet(mesh, x, y, z, speed, targetType, damage) {
		type = 1; // Neutron Gun type
		this->offset = offset;
	}

	Bullet* Clone() const override {
		return new NeutronGun(*this);
	}

	glm::vec2 getCenter() override;
};