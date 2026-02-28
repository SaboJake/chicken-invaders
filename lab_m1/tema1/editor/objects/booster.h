#pragma once

#include "lab_m1/tema1/editor/placed.h"

class Booster : public Placed {
public:
	Booster(std::vector<Mesh*> meshes, std::vector<glm::mat4> offsets, glm::mat4 gridOffset, float x, float y, float z)
		: Placed(meshes, offsets, gridOffset, x, y, z) {
		type = 3;
	}
	Booster(Mesh* mesh, float x, float y, float z)
		: Placed(mesh, x, y, z) {
		type = 3;
	}
	void SetSlots(Grid* grid, int i, int j, bool set) override;
	bool PlaceCheck(Grid* grid, int i, int j) override;
	bool clickCheck(float mouseX, float mouseY, float unit) override;

	Placed* Clone() const override {
		return new Booster(*this);
	}
};