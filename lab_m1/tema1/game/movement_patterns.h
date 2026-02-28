#pragma once
#include "lab_m1/tema1/game/enemies/chicken.h"
#include <cmath>
#include <functional>

namespace MovementPatterns {
	// Stationary movement strategy
    static std::function<void(Chicken&, float)> Stationary = [](Chicken& chicken, float deltaTime) {
        // Do nothing
		};

    // Sine wave movement strategy
    static std::function<void(Chicken&, float)> SineWave = [](Chicken& chicken, float deltaTime) {
        chicken.x += chicken.moveDir * 3 * chicken.speed * deltaTime;
        chicken.y += 100.0f * std::sinf(chicken.x / 100.0f) * deltaTime;

        // Reverse direction if hitting screen edges
        if (chicken.x < 64) {
            chicken.moveDir = 1;
        } else if (chicken.x > 1920 - 64) {
            chicken.moveDir = -1;
		}
    };

    // Linear downward movement strategy
    static std::function<void(Chicken&, float)> LinearDownward = [](Chicken& chicken, float deltaTime) {
        chicken.y += chicken.speed * deltaTime;
    };

	// Sine wave downward movement
    static std::function<void(Chicken&, float)> SineWaveDownward = [](Chicken& chicken, float deltaTime) {
        chicken.y += chicken.speed * deltaTime;
        chicken.x += 400.0f * std::sinf(chicken.y) * deltaTime;
    };

    // Slight left-right oscillation
    static std::function<void(Chicken&, float)> LeftRightOscillation1 = [](Chicken& chicken, float deltaTime) {
		chicken.x += chicken.moveDir * chicken.speed * deltaTime;
        if (chicken.x < chicken.startX - 100.0f) {
            chicken.moveDir = 1;
        } else if (chicken.x > chicken.startX + 100.0f) {
            chicken.moveDir = -1;
		}
	};
	// Square movement pattern
    static std::function<void(Chicken&, float)> SquarePattern1 = [](Chicken& chicken, float deltaTime) {
        float sideLength = 200.0f;
        float speed = chicken.speed;
        if (chicken.moveDir == 1) { // Move right
            chicken.x += speed * deltaTime;
            if (chicken.x >= chicken.startX + sideLength) {
                chicken.moveDir = 2;
            }
        } else if (chicken.moveDir == 2) { // Move down
            chicken.y += speed * deltaTime;
            if (chicken.y >= chicken.startY + sideLength) {
                chicken.moveDir = 3;
            }
        } else if (chicken.moveDir == 3) { // Move left
            chicken.x -= speed * deltaTime;
            if (chicken.x <= chicken.startX) {
                chicken.moveDir = 4;
            }
        } else if (chicken.moveDir == 4) { // Move up
            chicken.y -= speed * deltaTime;
            if (chicken.y <= chicken.startY) {
                chicken.moveDir = 1; // Repeat the pattern
            }
        }
		};
}