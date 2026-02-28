#include "lab_m1/tema1/editor/placed.h"
#include "lab_m1/tema1/editor/grid.h"

#include <iostream>

void Placed::Drag(int mouseX, int mouseY) {
    x = static_cast<float>(mouseX);
    y = static_cast<float>(mouseY);
    isDragging = true;
}

void Placed::SetSlots(Grid* grid, int i, int j, bool set) {
	std::cout << "Placed::SetSlots called" << std::endl;
	grid->slots[i][j].isFilled = set;
	grid->slots[i][j].type = set ? 1 : 0;
}

bool Placed::PlaceCheck(Grid* grid, int i, int j) {
	std::cout << "Placed::PlaceCheck called" << std::endl;
	if (i < 0 || i >= grid->rows || j < 0 || j >= grid->cols) {
		return false;
	}
	if (grid->slots[i][j].isFilled) {
		return false;
	}
	// Search downwards for bumber or cannon
	for (int row = i + 1; row < grid->rows; row++) {
		if (grid->slots[row][j].type == 2 || grid->slots[row][j].type == 4) {
			return false;
		}
	}
	// Search upwards for engine
	for (int row = i - 1; row >= 0; row--) {
		if (grid->slots[row][j].type == 3) {
			return false;
		}
	}
	return true;
}

glm::vec2 Placed::SearchGridPosition(Grid* grid, int i, int j) {
	return glm::vec2(i, j);
}

bool Placed::clickCheck(float mouseX, float mouseY, float unit) {
	if (mouseX >= x && mouseX <= x + unit &&
		mouseY >= y && mouseY <= y + unit) {
		return true;
	}
	return false;
}