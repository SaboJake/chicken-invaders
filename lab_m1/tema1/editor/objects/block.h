#pragma once

#include "lab_m1/tema1/editor/placed.h"

class Block : public Placed {
	public:
	Block(std::vector<Mesh*> meshes, std::vector<glm::mat4> offsets, glm::mat4 gridOffset, float x, float y, float z)
		: Placed(meshes, offsets, gridOffset, x, y, z) {
	}
	Block(Mesh* mesh, float x, float y, float z)
		: Placed(mesh, x, y, z) {
	}
	Placed* Clone() const override {
		return new Block(*this);
	}
};