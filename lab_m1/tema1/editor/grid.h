#pragma once

#include "lab_m1/tema1/editor/slot.h"
#include <vector>

using std::vector;

class Grid {
public:
    int l;
    int rows;
    int cols;
    float space_x;
    float space_y;
    Mesh* square; // Shared Mesh for all slots
    vector<vector<Slot>> slots;

    Grid(int l, int rows, int cols, float space_x, float space_y, Mesh* square)
        : l(l), rows(rows), cols(cols), space_x(space_x), space_y(space_y), square(square) {
        slots.resize(rows);
        for (int i = 0; i < rows; i++) {
            slots[i].resize(cols);
            for (int j = 0; j < cols; j++) {
                slots[i][j].x = j * space_x;
                slots[i][j].y = i * space_y;
                slots[i][j].z = 0;
                slots[i][j].square = square;
                slots[i][j].isFilled = false;
				slots[i][j].connected = false;
                slots[i][j].object = nullptr;
				slots[i][j].type = 0; // empty
            }
        }
    }

    glm::vec2 SearchObject(int type, int i, int j);

    bool checkConnected();
};