#include "lab_m1/tema1/editor/objects/booster.h"
#include "lab_m1/tema1/editor/grid.h"

void Booster::SetSlots(Grid* grid, int i, int j, bool set) {
	// Booster is 2 tiles tall and 1 tile wide
	grid->slots[i][j].isFilled = set;
	grid->slots[i + 1][j].isFilled = set;

	grid->slots[i][j].type = set ? 3 : 0; // 3 for Booster
	grid->slots[i + 1][j].type = set ? 3 : 0;
}

bool Booster::PlaceCheck(Grid* grid, int i, int j) {
	// Check if the Booster fits within the grid bounds
	if (i + 1 >= grid->rows || j < 0 || j >= grid->cols) {
		return false;
	}
	if (grid->slots[i][j].isFilled ||
		grid->slots[i + 1][j].isFilled) {
		return false;
	}
	for (int row = i + 2; row < grid->rows; row++) {
		if (grid->slots[row][j].isFilled) {
			return false;
		}
	}
	return __super::PlaceCheck(grid, i, j);
}

bool Booster::clickCheck(float mouseX, float mouseY, float unit) {
	if (mouseX >= x && mouseX <= x + unit &&
		mouseY >= y && mouseY <= y + 2 * unit) {
		return true;
	}
	return false;
}