#pragma once

#include <vector>
#include "components/simple_scene.h"

class Dragable {
public:
	std::vector<Mesh*> meshes;
	float x;
	float y;
	float z;
	bool isDragging;

	Dragable(std::vector<Mesh*> meshes, float x, float y, float z)
		: meshes(meshes), x(x), y(y), z(z), isDragging(false) {
	}

	Dragable(Mesh* mesh, float x, float y, float z)
		: x(x), y(y), z(z), isDragging(false) {
		meshes.push_back(mesh);
	}

	void Drag(int mouseX, int mouseY);
};