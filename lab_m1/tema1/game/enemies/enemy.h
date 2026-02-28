#pragma once

#include <vector>
#include "core/gpu/mesh.h"

class Enemy {
public:
	std::vector<Mesh*> meshes;
	std::vector<glm::mat4> offsets;
	float x;
	float y;
	float z;

	float startX;
	float startY;
	float startZ;

	float speed;
	bool isActive;

	int health;

	glm::vec2 hitboxR;

	int type;

	virtual ~Enemy() = default;

	virtual Enemy* Clone() const = 0;

	Enemy(std::vector<Mesh*> meshes, int type, float x, float y, float z, float speed, int health)
		: meshes(meshes), type(type), x(x), y(y), z(z), speed(speed), isActive(true), hitboxR(glm::vec2(1.0f)), health(health) {
		startX = x;
		startY = y;
		startZ = z;
		for (size_t i = 0; i < meshes.size(); i++) {
			offsets.push_back(glm::mat4(1.0f));
		}
	}

	Enemy(std::vector<Mesh*> meshes, std::vector<glm::mat4> offsets, int type, float x, float y, float z, float speed, int health)
		: meshes(meshes), type(type), offsets(offsets), x(x), y(y), z(z), speed(speed), isActive(true), hitboxR(glm::vec2(1.0f)), health(health) {
		startX = x;
		startY = y;
		startZ = z;
	}

	virtual void Move(float deltaTime);
	virtual bool hitCheck(float x, float y);
	virtual glm::vec2 getCenter();
	virtual void OffscreenCheck(float width, float height);
};