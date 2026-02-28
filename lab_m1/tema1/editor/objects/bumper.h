#pragma once

#include "lab_m1/tema1/editor/placed.h"

class Bumper : public Placed {
public:
	Bumper(std::vector<Mesh*> meshes, std::vector<glm::mat4> offsets, glm::mat4 gridOffset, float x, float y, float z)
		: Placed(meshes, offsets, gridOffset, x, y, z) {
		type = 4;
	}
	Bumper(Mesh* mesh, float x, float y, float z)
		: Placed(mesh, x, y, z) {
		type = 4;
	}
	void SetSlots(Grid* grid, int i, int j, bool set) override;
	bool PlaceCheck(Grid* grid, int i, int j) override;
	glm::vec2 SearchGridPosition(Grid* grid, int i, int j) override;
	bool clickCheck(float mouseX, float mouseY, float unit) override;
	Placed* Clone() const override {
		return new Bumper(*this);
	}
	bool topDestroyed = false;
};