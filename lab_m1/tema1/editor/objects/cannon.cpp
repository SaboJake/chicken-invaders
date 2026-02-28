#include "lab_m1/tema1/editor/objects/cannon.h"
#include "lab_m1/tema1/editor/grid.h"

#include <iostream>

void Cannon::SetSlots(Grid* grid, int i, int j, bool set) {
    // Cannon is 3 tiles tall and 1 tile wide
	std::cout << "Cannon::SetSlots called" << std::endl;
    grid->slots[i][j].isFilled = set;
    grid->slots[i - 1][j].isFilled = set;
    grid->slots[i - 2][j].isFilled = set;

	grid->slots[i][j].type = set ? 2 : 0; // 2 for cannon
	grid->slots[i - 1][j].type = set ? 2 : 0;
	grid->slots[i - 2][j].type = set ? 2 : 0;
}

bool Cannon::PlaceCheck(Grid* grid, int i, int j) {
    // Check if the cannon fits within the grid bounds
	std::cout << "Cannon::PlaceCheck called" << std::endl;
    if (i - 2 < 0 || j < 0 || j >= grid->cols) {
        return false;
    }
    if (grid->slots[i][j].isFilled ||
        grid->slots[i - 1][j].isFilled ||
        grid->slots[i - 2][j].isFilled) {
        return false;
	}

    for (int row = i - 3; row >= 0; row--) {
        if (grid->slots[row][j].isFilled) {
            return false;
        }
	}

	return __super::PlaceCheck(grid, i, j);
}

glm::vec2 Cannon::SearchGridPosition(Grid* grid, int i, int j) {
	// search starting form (i, j) downwards
    for (int row = i; row >= 0; row--) {
        if (grid->slots[row][j].type == 2 && grid->slots[row][j].object) {
			return glm::vec2(row, j);
        }
    }
}

bool Cannon::clickCheck(float mouseX, float mouseY, float unit) {
    if (mouseX >= x && mouseX <= x + unit &&
        mouseY >= y - 2 * unit && mouseY <= y + unit) {
        return true;
    }
    return false;
}