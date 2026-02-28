#pragma once

#include "core/gpu/mesh.h"
#include "utils/gl_utils.h"

class Bullet {
public:
	Mesh* mesh;
	glm::mat4 offset;
	float x;
	float y;
	float z;

	float speed;
	int targetType;
	int type;

	int damage;

	bool isActive;

	virtual ~Bullet() = default;
	Bullet(Mesh* mesh, float x, float y, float z, float speed, int targetType, int damage)
		: mesh(mesh), offset(glm::mat4(1)), x(x), y(y), z(z), speed(speed), targetType(targetType), isActive(true), damage(damage), type(0) {
	}

	virtual Bullet* Clone() const = 0;

	virtual void Move(float deltaTime);

	virtual glm::vec2 getCenter();

	void Despawn(int screenWidth, int screenHeight);
};