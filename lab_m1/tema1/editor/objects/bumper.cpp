#include "lab_m1/tema1/editor/objects/bumper.h"
#include "lab_m1/tema1/editor/grid.h"

void Bumper::SetSlots(Grid* grid, int i, int j, bool set) {
	// Bumper is T shaped
	grid->slots[i][j].isFilled = set;
	grid->slots[i - 1][j].isFilled = set;
	grid->slots[i - 1][j - 1].isFilled = set;
	grid->slots[i - 1][j + 1].isFilled = set;

	grid->slots[i][j].type = set ? 4 : 0; // 4 for Bumper
	grid->slots[i - 1][j].type = set ? 4 : 0;
	grid->slots[i - 1][j - 1].type = set ? 4 : 0;
	grid->slots[i - 1][j + 1].type = set ? 4 : 0;
}

bool Bumper::PlaceCheck(Grid* grid, int i, int j) {
	// Check if the Bumper fits within the grid bounds
	if (i - 1 < 0 || j - 1 < 0 || j + 1 >= grid->cols) {
		return false;
	}
	if (grid->slots[i][j].isFilled ||
		grid->slots[i - 1][j].isFilled ||
		grid->slots[i - 1][j - 1].isFilled ||
		grid->slots[i - 1][j + 1].isFilled) {
		return false;
	}

	for (int row = i - 2; row >= 0; row--) {
		if (grid->slots[row][j].isFilled
			|| grid->slots[row][j - 1].isFilled
			|| grid->slots[row][j + 1].isFilled) {
			return false;
		}
	}

	return __super::PlaceCheck(grid, i - 1, j)
		&& __super::PlaceCheck(grid, i - 1, j - 1)
		&& __super::PlaceCheck(grid, i - 1, j + 1);
}

glm::vec2 Bumper::SearchGridPosition(Grid* grid, int i, int j) {
	// Check if the current slot is a bumper base
	if (grid->slots[i][j].type == 4 && grid->slots[i][j].object) {
		return glm::vec2(i, j);
	}

	// Search down-left, down, and down-right for the base of the bumper
	if (i + 1 < grid->rows) {
		for (int col = j - 1; col <= j + 1; col++) {
			if (col >= 0 && col < grid->cols) {
				if (grid->slots[i + 1][col].type == 4 && grid->slots[i + 1][col].object) {
					return glm::vec2(i + 1, col);
				}
			}
		}
	}

	// Return invalid position if not found
	return glm::vec2(-1, -1);
}

bool Bumper::clickCheck(float mouseX, float mouseY, float unit) {
	if (topDestroyed) {
		return __super::clickCheck(mouseX, mouseY, unit);
	}
	if (__super::clickCheck(mouseX, mouseY, unit)) {
		return true;
	}
	if (mouseX >= x - unit && mouseX <= x + 2 * unit &&
		mouseY >= y - unit && mouseY <= y) {
		return true;
	}
	return false;
}