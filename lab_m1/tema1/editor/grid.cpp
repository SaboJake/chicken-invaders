#include "lab_m1/tema1/editor/grid.h"

glm::vec2 Grid::SearchObject(int type, int i, int j) {
    switch (type) {
    case 1: // block
        return glm::vec2(i, j);
    case 2: // cannon
        // search starting from (i, j) downwards
        for (int row = i; row < rows; row++) {
            if (slots[row][j].type == 2 && slots[row][j].object) {
                return glm::vec2(row, j);
            }
        }
        break;
    case 3: // engine
        // search starting from (i, j) upwards
        for (int row = i; row >= 0; row--) {
            if (slots[row][j].type == 3 && slots[row][j].object) {
                return glm::vec2(row, j);
            }
        }
        break;
    case 4: // bumper
        // is base of bumper
        if (slots[i][j].type == 4 && slots[i][j].object) {
            return glm::vec2(i, j);
        }
        // search down-left, down and down-right for base of bumper
        if (i + 1 < rows) {
            for (int col = j - 1; col <= j + 1; col++) {
                if (col >= 0 && col < cols) {
                    if (slots[i + 1][col].type == 4 && slots[i + 1][col].object) {
                        return glm::vec2(i + 1, col);
                    }
                }
            }
        }
        break;
    default:
        break;
    }
    // Return invalid position if not found
    return glm::vec2(-1, -1);
}

bool Grid::checkConnected() {
	std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
	int cnt = 0;

    std::function<void(int, int)> dfs = [&](int i, int j) {
        if (i < 0 || i >= rows || j < 0 || j >= cols) return;
        if (visited[i][j] || !slots[i][j].connected) return;
        visited[i][j] = true;

        dfs(i + 1, j);
        dfs(i - 1, j);
        dfs(i, j + 1);
        dfs(i, j - 1);
	};

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (slots[i][j].connected && !visited[i][j]) {
                cnt++;

                if (cnt > 1) {
                    return false;
				}
				dfs(i, j);
            }
        }
	}

    return cnt == 1;
}