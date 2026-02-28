#pragma once

#include "lab_m1/tema1/game/projectiles/bullet.h"
#include "lab_m1/tema1/my_utils/transform3D.h"

class Egg : public Bullet {
public:
    Egg(Mesh* mesh, float x, float y, float z, float speed = 400.0f)
        : Bullet(mesh, x, y, z, speed, 2, 1) { // targetType 2 = targets player
		type = -1; // Egg type
		offset = transform3D::Scale(1.0f, 1.5f, 1.0f);
    }
    Bullet* Clone() const override {
        return new Egg(*this);
    }

    void Move(float deltaTime) override;
};