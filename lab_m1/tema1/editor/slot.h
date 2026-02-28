#pragma once

#include "components/simple_scene.h"

class Placed;

class Slot {
public:
    Mesh* square;
    float x;
    float y;
    float z;
    Placed* object;
    bool isFilled;
	int type; // 0 - empty, 1 - block, 2 - cannon, 3 - engine, 4 - bumper
	bool connected;
};